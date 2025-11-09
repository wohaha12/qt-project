#ifndef DATABASECONNECTIONPOOL_H
#define DATABASECONNECTIONPOOL_H

#include <QObject>
#include <QSqlDatabase>
#include <QQueue>
#include <QMutex>
#include <QString>

class DatabaseConnectionPool : public QObject
{
    Q_OBJECT
public:
    static DatabaseConnectionPool& getInstance();
    ~DatabaseConnectionPool();

    // 获取数据库连接
    QSqlDatabase getConnection();

    // 释放数据库连接
    void releaseConnection(const QSqlDatabase& connection);

    // 关闭所有连接并清空连接池
    void clear();

    // 获取当前连接池信息
    int getConnectionCount();
    int getFreeConnectionCount();

    // 初始化连接池 - 移到public部分并更新签名
    void init(const QString& driver, const QString& hostName,
              quint16 port, const QString& databaseName,
              const QString& userName, const QString& password);

private:
    explicit DatabaseConnectionPool(QObject *parent = nullptr);
    DatabaseConnectionPool(const DatabaseConnectionPool&) = delete;
    DatabaseConnectionPool& operator=(const DatabaseConnectionPool&) = delete;

    // 创建新的数据库连接
    QSqlDatabase createConnection();

    // 检查连接是否有效
    bool isValid(const QSqlDatabase& connection);

    // 连接池配置参数
    QString m_hostName;          // 数据库主机名
    quint16 m_port;              // 数据库端口
    QString m_databaseName;      // 数据库名
    QString m_userName;          // 用户名
    QString m_password;          // 密码
    QString m_driver;            // 数据库驱动

    // 连接池属性
    int m_maxConnections;        // 最大连接数
    int m_initialConnections;    // 初始连接数
    int m_connectionTimeout;     // 连接超时时间(ms)

    // 连接池状态
    QQueue<QString> m_freeConnections;    // 空闲连接队列
    QMutex m_mutex;                       // 互斥锁，保证线程安全
    QMutex m_connectionMutex;             // 连接创建互斥锁

    // 连接名称前缀
    static const QString CONNECTION_NAME_PREFIX;
};

#endif // DATABASECONNECTIONPOOL_H
