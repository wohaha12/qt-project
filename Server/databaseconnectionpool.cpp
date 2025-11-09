#include "databaseconnectionpool.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QThread>


const QString DatabaseConnectionPool::CONNECTION_NAME_PREFIX = "qt_database_connection_";

DatabaseConnectionPool::DatabaseConnectionPool(QObject *parent) : QObject(parent)
{

}

DatabaseConnectionPool::~DatabaseConnectionPool()
{
    clear();
}

DatabaseConnectionPool& DatabaseConnectionPool::getInstance()
{
    static DatabaseConnectionPool instance;
    return instance;
}

void DatabaseConnectionPool::init(const QString& driver, const QString& hostName,
                                  quint16 port, const QString& databaseName,
                                  const QString& userName, const QString& password)
{
    // 设置默认配置参数
    m_hostName = hostName;
    m_port = port;
    m_databaseName = databaseName;
    m_userName = userName;
    m_password = password;
    m_driver = driver;

    // 设置连接池属性
    m_maxConnections = 10;
    m_initialConnections = 3;
    m_connectionTimeout = 5000;

    // 预创建初始连接
    for (int i = 0; i < m_initialConnections; ++i) {
        QSqlDatabase connection = createConnection();
        if (connection.isOpen()) {
            QString connectionName = connection.connectionName();
            m_freeConnections.enqueue(connectionName);
        }
    }
}

QSqlDatabase DatabaseConnectionPool::createConnection()
{
    // 使用互斥锁保证连接创建的线程安全
    QMutexLocker locker(&m_connectionMutex);

    // 生成唯一的连接名称
    static int connectionCount = 0;
    QString connectionName = CONNECTION_NAME_PREFIX + QString::number(++connectionCount);

    // 检查是否已经存在该名称的连接
    if (QSqlDatabase::contains(connectionName)) {
        QSqlDatabase existingDb = QSqlDatabase::database(connectionName);
        if (isValid(existingDb)) {
            return existingDb;
        } else {
            // 连接无效，移除并重新创建
            QSqlDatabase::removeDatabase(connectionName);
        }
    }

    // 创建新的数据库连接
    QSqlDatabase db = QSqlDatabase::addDatabase(m_driver, connectionName);
    db.setHostName(m_hostName);
    db.setPort(m_port);
    db.setDatabaseName(m_databaseName);
    db.setUserName(m_userName);
    db.setPassword(m_password);

    // 打开连接
    if (!db.open()) {
        qDebug() << "创建数据库连接失败：" << db.lastError().text();
        return QSqlDatabase();
    }

    qDebug() << "创建数据库连接成功：" << connectionName;
    return db;
}

QSqlDatabase DatabaseConnectionPool::getConnection()
{
    QMutexLocker locker(&m_mutex);

    // 优先从空闲连接队列中获取连接
    if (!m_freeConnections.isEmpty()) {
        QString connectionName = m_freeConnections.dequeue();

        // 检查连接是否有效
        if (QSqlDatabase::contains(connectionName)) {
            QSqlDatabase connection = QSqlDatabase::database(connectionName);
            if (isValid(connection)) {
                return connection;
            } else {
                // 连接无效，移除并创建新连接
                QSqlDatabase::removeDatabase(connectionName);
                return createConnection();
            }
        }
    }

    // 如果没有空闲连接，且未达到最大连接数，则创建新连接
    QStringList allConnections = QSqlDatabase::connectionNames();
    int currentConnectionCount = 0;

    foreach (const QString& name, allConnections) {
        if (name.startsWith(CONNECTION_NAME_PREFIX)) {
            currentConnectionCount++;
        }
    }

    if (currentConnectionCount < m_maxConnections) {
        return createConnection();
    }

    // 如果已经达到最大连接数，则等待连接释放（这里简单实现，实际项目中可以添加超时机制）
    qDebug() << "警告：数据库连接池已满，等待连接释放...";

    // 简单实现：等待一段时间后重试
    locker.unlock(); // 解锁，允许其他线程操作连接池
    QThread::msleep(100); // 等待100毫秒
    locker.relock(); // 重新加锁

    // 再次尝试获取连接
    if (!m_freeConnections.isEmpty()) {
        QString connectionName = m_freeConnections.dequeue();
        if (QSqlDatabase::contains(connectionName)) {
            QSqlDatabase connection = QSqlDatabase::database(connectionName);
            if (isValid(connection)) {
                return connection;
            }
        }
    }

    // 仍然无法获取连接，创建一个临时连接（超出池大小）
    qWarning() << "警告：创建临时数据库连接（超出池大小限制）";
    QSqlDatabase tempConnection = createConnection();
    if(!tempConnection.isValid()) {
        qCritical() << "错误：无法创建数据库连接，连接池已满且无法创建新连接";
    }
    return tempConnection;
}

void DatabaseConnectionPool::releaseConnection(const QSqlDatabase& connection)
{
    if (!connection.isValid()) {
        return;
    }

    QString connectionName = connection.connectionName();
    if (!connectionName.startsWith(CONNECTION_NAME_PREFIX)) {
        return; // 不是连接池创建的连接，不处理
    }

    QMutexLocker locker(&m_mutex);

    // 检查连接是否已经在空闲队列中
    if (!m_freeConnections.contains(connectionName)) {
        m_freeConnections.enqueue(connectionName);
    }
}

void DatabaseConnectionPool::clear()
{
    QMutexLocker locker(&m_mutex);

    // 关闭并移除所有连接
    QStringList allConnections = QSqlDatabase::connectionNames();

    foreach (const QString& name, allConnections) {
        if (name.startsWith(CONNECTION_NAME_PREFIX)) {
            {
                QSqlDatabase db = QSqlDatabase::database(name);
                db.close();
            }
            QSqlDatabase::removeDatabase(name);
            qDebug() << "关闭并移除数据库连接：" << name;
        }
    }

    // 清空空闲连接队列
    m_freeConnections.clear();
}

bool DatabaseConnectionPool::isValid(const QSqlDatabase& connection)
{
    if (!connection.isValid() || !connection.isOpen()) {
        return false;
    }

    // 执行简单查询测试连接是否有效
    QSqlQuery query(connection);
    return query.exec("SELECT 1");
}

int DatabaseConnectionPool::getConnectionCount()
{
    QStringList allConnections = QSqlDatabase::connectionNames();
    int count = 0;

    foreach (const QString& name, allConnections) {
        if (name.startsWith(CONNECTION_NAME_PREFIX)) {
            count++;
        }
    }

    return count;
}

int DatabaseConnectionPool::getFreeConnectionCount()
{
    QMutexLocker locker(&m_mutex);
    return m_freeConnections.size();
}
