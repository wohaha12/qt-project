//#include "operatedb.h"
//#include <QDebug>
//#include <QSqlError>
//#include <QSqlQuery>
//OperateDB::OperateDB(QObject *parent) : QObject(parent)
//{
//    m_db = QSqlDatabase::addDatabase("QMYSQL");
//}

//OperateDB::~OperateDB()
//{
//    m_db.close();
//}

//void OperateDB::connect()
//{
//    m_db.setHostName("localhost");
//    m_db.setPort(3307);
//    m_db.setDatabaseName("mydbqt");
//    m_db.setUserName("root");
//    m_db.setPassword("123456");
//    if(m_db.open()){
//        qDebug()<<"数据库连接成功";
//    }else{
//        qDebug()<<"数据库连接失败"<<m_db.lastError().text();
//    }
//}

//OperateDB &OperateDB::getInstance()
//{
//    static OperateDB instance;
//    return instance;
//}

//bool OperateDB::handleRegist(const char *caName, const char *caPwd)
//{
//    if (caName == nullptr || caPwd == nullptr) {
//        qDebug() << "注册失败：用户名或密码为空";
//        return false;
//    }
//    QString sql = QString("select * from user_info where name='%1'").arg(caName);
//    qDebug()<<"查询用户是否存在"<<sql;
//    QSqlQuery q;
////    q.exec();执行sql语句，返回值代表是否执行成功
////    q.next();取出结果集中的第一个结果，返回值代表是否存在
//    if(!q.exec(sql) || q.next()) return false;
//    sql = QString("insert into user_info(name,pwd) values('%1','%2')").arg(caName,caPwd);
//    qDebug()<<"插入用户数据"<<sql;
//    return q.exec(sql);
//}

//bool OperateDB::handleLogin(const char *caName, const char *caPwd)
//{
//    if (caName == nullptr || caPwd == nullptr) {
//        qDebug() << "登录失败：用户名或密码为空";
//        return false;
//    }
//    QString sql = QString("select pwd from user_info where name = '%1'").arg(caName);
//    qDebug() << "查询登录用户是否存在" << sql;
//    QSqlQuery q;
//    if (!q.exec(sql)) {
//        qDebug() << "查询登录用户失败：" << q.lastError().text();
//        return false;
//    }
//    // 检查用户是否存在
//    if (!q.next()) {
//        qDebug() << "登录失败：用户名" << caName << "不存在";
//        return false;
//    }
//    // 从查询结果中获取存储的密码（第0列是pwd字段）
//    QString storedPwd = q.value(0).toString();
//    if (storedPwd != QString(caPwd)) {
//        qDebug() << "登录失败：密码错误 for user" << caName;
//        return false;
//    }
//    sql = QString("update user_info set online = 1 where name = '%1' and pwd = '%2'").arg(caName,caPwd);
//    qDebug() << "更新用户在线状态" << sql;
//    if(!q.exec(sql)){
//        qDebug() << "更新用户在线状态失败" + q.lastError().text();
//        return false;
//    }
//    qDebug() << "登录成功：用户名" << caName;
//    return true;
//}

//void OperateDB::handleOfflint(const char *caName)
//{
//    if(caName == NULL){
//        return;
//    }
//    QString sql = QString("update user_info set online = 0 where name = '%1'").arg(caName);
//    QSqlQuery q;
//    qDebug()<<"用户在线状态置为0"<<sql;
//    q.exec(sql);
//}

//int OperateDB::handleFindUser(const char *caName)
//{
//    if (caName == NULL) {
//        return -1;
//    }

//    QString sql = QString("select online from user_info where name = '%1'").arg(caName);
//    QSqlQuery q;
//    qDebug() << "通过用户名查找online字段" << sql;
//    if(!q.exec(sql)){
//        qDebug() << "查找用户失败：" << q.lastError().text();
//        return -1;
//    }

//    if(q.next()){
//        return q.value(0).toInt();
//    }
//    return 2;
//}

//QStringList OperateDB::handleOnlineUser()
//{
//    QStringList res;
//    QString sql = QString("select name from user_info where online = 1");
//    QSqlQuery q;
//    q.exec(sql);
//    while (q.next()) {
//        res.append(q.value(0).toString());
//    }
//    return res;
//}

//int OperateDB::handleAddFriend(const char *curName, const char *tarName)
//{
//    if (curName == NULL || tarName == NULL) {
//        return -1;
//    }
//    // 查询好友表，判断是否已是好友
//    QString sql = QString(R"(
//      select * from friend
//      where
//      (
//        user_id = (select id from user_info where name='%1')
//        and
//        friend_id = (select id from user_info where name='%2')
//      )
//      or
//      (
//        user_id = (select id from user_info where name='%2')
//        and
//        friend_id = (select id from user_info where name='%1')
//      )
//    )").arg(curName).arg(tarName);
//    qDebug()<<"查询是否为好友sql"<<sql;
//    QSqlQuery q;
//    q.exec(sql);
//    if (q.next()) {
//        return 2;  // 已是好友
//    }
//    // 查询目标用户是否在线
//    sql = QString("select online from user_info where name = '%1'").arg(tarName);
//    qDebug()<<"查询目标用户是否在线sql"<<sql;
//    q.exec(sql);
//    if(q.next()){
//        return q.value(0).toInt();
//    }
//    return -1;
//}

//bool OperateDB::handleAgreeAddFriend(const char *curName, const char *tarName)
//{
//    if (curName == NULL || tarName == NULL) {
//        return false;
//    }
//    QString sql = QString(R"(
//        insert into friend(user_id, friend_id)
//        select u1.id,u2.id
//        from user_info u1,user_info u2
//        where u1.name = '%1' and u2.name = '%2'
//        )").arg(curName).arg(tarName);
//    qDebug()<<"添加好友记录"<<sql;
//    QSqlQuery q;
//    return q.exec(sql);
//}

//QStringList OperateDB::handleFlushOnlineUser(const char *caName)
//{
//    QStringList res;
//    if (caName == NULL) {
//        return res;
//    }
//    QString sql = QString(R"(
//         select name from user_info
//         where id in
//          (
//            select user_id from friend where
//            friend_id=(select id from user_info where name='%1')
//            union
//            select friend_id from friend where
//            user_id=(select id from user_info where name='%1')
//          )
//          and online=1;
//                          )").arg(caName);

//    QSqlQuery q;
//    q.exec(sql);
//    while (q.next()) {
//        res.append(q.value(0).toString());
//    }
//    return res;
//}

//bool OperateDB::handleDelFriend(const char *curName, const char *tarName)
//{
//    if (curName == NULL || tarName == NULL) {
//        return false;
//    }
//    // 查询好友表，判断是否已是好友
//    QString sql = QString(R"(
//      select * from friend
//      where
//      (
//        user_id = (select id from user_info where name='%1')
//        and
//        friend_id = (select id from user_info where name='%2')
//      )
//      or
//      (
//        user_id = (select id from user_info where name='%2')
//        and
//        friend_id = (select id from user_info where name='%1')
//      )
//    )").arg(curName).arg(tarName);
//    qDebug()<<"查询是否为好友sql"<<sql;
//    QSqlQuery q;
//    q.exec(sql);
//    if (!q.next()) {
//        return false;
//    }
//    sql = QString(R"(
//      delete from friend
//      where
//      (
//        user_id = (select id from user_info where name='%1')
//        and
//        friend_id = (select id from user_info where name='%2')
//      )
//      or
//      (
//        user_id = (select id from user_info where name='%2')
//        and
//        friend_id = (select id from user_info where name='%1')
//      )
//    )").arg(curName).arg(tarName);
//    qDebug()<<"删除好友sql"<<sql;
//    return q.exec(sql);
//}



#include "operatedb.h"
#include <QDebug>
#include <QFileInfo>
#include <QSqlError>
#include <QSqlQuery>
#include "databaseconnectionpool.h"
#include "QDir"

OperateDB::OperateDB(QObject *parent) : QObject(parent), m_inTransaction(false)
{
    // 构造函数中不再初始化数据库连接，而是在需要时从连接池获取
}

OperateDB::~OperateDB()
{
    // 析构函数中不再关闭数据库连接
}

void OperateDB::connect()
{
    // 这个方法现在只用于初始化连接池
    // 实际连接的创建和管理由DatabaseConnectionPool负责
    qDebug() << "数据库连接池已初始化";
}

OperateDB &OperateDB::getInstance()
{
    static OperateDB instance;
    return instance;
}

// 执行SQL查询并返回结果
QSqlQuery OperateDB::executeQuery(const QString& sql)
{
    // 从连接池获取连接
    QSqlDatabase db = DatabaseConnectionPool::getInstance().getConnection();
    QSqlQuery query(db);

    qDebug() << "执行查询SQL:" << sql;
    if (!query.exec(sql)) {
        qDebug() << "SQL执行失败:" << query.lastError().text();
    }

    // 如果不在事务中，查询完成后归还连接
    if (!m_inTransaction) {
        DatabaseConnectionPool::getInstance().releaseConnection(db);
    }

    return query;
}

// 执行SQL更新（插入、更新、删除）并返回结果
bool OperateDB::executeUpdate(const QString& sql)
{
    // 从连接池获取连接
    QSqlDatabase db = DatabaseConnectionPool::getInstance().getConnection();
    QSqlQuery query(db);

    qDebug() << "执行更新SQL:" << sql;
    bool result = query.exec(sql);

    if (!result) {
        qDebug() << "SQL执行失败:" << query.lastError().text();
    }

    // 如果不在事务中，操作完成后归还连接
    if (!m_inTransaction) {
        DatabaseConnectionPool::getInstance().releaseConnection(db);
    }

    return result;
}

// 开始事务
bool OperateDB::beginTransaction()
{
    if (!m_inTransaction) {
        m_currentConnection = DatabaseConnectionPool::getInstance().getConnection();
        m_inTransaction = m_currentConnection.transaction();
        return m_inTransaction;
    }
    return false;
}

// 提交事务
bool OperateDB::commitTransaction()
{
    if (m_inTransaction) {
        bool result = m_currentConnection.commit();
        DatabaseConnectionPool::getInstance().releaseConnection(m_currentConnection);
        m_inTransaction = false;
        return result;
    }
    return false;
}

// 回滚事务
bool OperateDB::rollbackTransaction()
{
    if (m_inTransaction) {
        bool result = m_currentConnection.rollback();
        DatabaseConnectionPool::getInstance().releaseConnection(m_currentConnection);
        m_inTransaction = false;
        return result;
    }
    return false;
}

bool OperateDB::handleRegist(const char *caName, const char *caPwd)
{
    if (caName == nullptr || caPwd == nullptr) {
        qDebug() << "注册失败：用户名或密码为空";
        return false;
    }

    QString sql = QString("select * from user_info where name='%1'").arg(caName);
    QSqlQuery q = executeQuery(sql);

    if (q.next()) return false;

    sql = QString("insert into user_info(name,pwd) values('%1','%2')").arg(caName,caPwd);
    return executeUpdate(sql);
}

bool OperateDB::handleLogin(const char *caName, const char *caPwd)
{
    if (caName == nullptr || caPwd == nullptr) {
        qDebug() << "登录失败：用户名或密码为空";
        return false;
    }

    QString sql = QString("select pwd from user_info where name = '%1'").arg(caName);
    QSqlQuery q = executeQuery(sql);

    // 检查用户是否存在
    if (!q.next()) {
        qDebug() << "登录失败：用户名" << caName << "不存在";
        return false;
    }

    // 从查询结果中获取存储的密码（第0列是pwd字段）
    QString storedPwd = q.value(0).toString();
    if (storedPwd != QString(caPwd)) {
        qDebug() << "登录失败：密码错误 for user" << caName;
        return false;
    }

    sql = QString("update user_info set online = 1 where name = '%1' and pwd = '%2'").arg(caName,caPwd);
    return executeUpdate(sql);
}

void OperateDB::handleOfflint(const char *caName)
{
    if(caName == NULL){
        return;
    }
    QString sql = QString("update user_info set online = 0 where name = '%1'").arg(caName);
    executeUpdate(sql);
}

int OperateDB::handleFindUser(const char *caName)
{
    if (caName == NULL) {
        return -1;
    }

    QString sql = QString("select online from user_info where name = '%1'").arg(caName);
    QSqlQuery q = executeQuery(sql);

    if(q.next()){
        return q.value(0).toInt();
    }
    return 2;
}

QStringList OperateDB::handleOnlineUser()
{
    QStringList res;
    QString sql = QString("select name from user_info where online = 1");
    QSqlQuery q = executeQuery(sql);

    while (q.next()) {
        res.append(q.value(0).toString());
    }
    return res;
}

int OperateDB::handleAddFriend(const char *curName, const char *tarName)
{
    if (curName == NULL || tarName == NULL) {
        return -1;
    }

    // 检查是否尝试添加自己为好友
    if (QString(curName) == QString(tarName)) {
        qDebug() << "不能添加自己为好友：" << curName;
        return -2;  // 返回-2表示不能添加自己
    }

    // 查询好友表，判断是否已是好友
    QString sql = QString(R"(
      select * from friend
      where
      (
        user_id = (select id from user_info where name='%1')
        and
        friend_id = (select id from user_info where name='%2')
      )
      or
      (
        user_id = (select id from user_info where name='%2')
        and
        friend_id = (select id from user_info where name='%1')
      )
    )").arg(curName).arg(tarName);

    QSqlQuery q = executeQuery(sql);
    if (q.next()) {
        return 2;  // 已是好友
    }

    // 查询目标用户是否在线
    sql = QString("select online from user_info where name = '%1'").arg(tarName);
    q = executeQuery(sql);

    if(q.next()){
        return q.value(0).toInt();
    }
    return -1;
}

bool OperateDB::handleAgreeAddFriend(const char *curName, const char *tarName)
{
    if (curName == NULL || tarName == NULL) {
        return false;
    }

    QString sql = QString(R"(
        insert into friend(user_id, friend_id)
        select u1.id,u2.id
        from user_info u1,user_info u2
        where u1.name = '%1' and u2.name = '%2'
        )").arg(curName).arg(tarName);

    return executeUpdate(sql);
}

QStringList OperateDB::handleFlushOnlineUser(const char *caName)
{
    QStringList res;
    if (caName == NULL) {
        return res;
    }

    QString sql = QString(R"(
         select name from user_info
         where id in
          (
            select user_id from friend where
            friend_id=(select id from user_info where name='%1')
            union
            select friend_id from friend where
            user_id=(select id from user_info where name='%1')
          )
          and online=1;
                          )").arg(caName);

    QSqlQuery q = executeQuery(sql);
    while (q.next()) {
        res.append(q.value(0).toString());
    }
    return res;
}

bool OperateDB::handleDelFriend(const char *curName, const char *tarName)
{
    if (curName == NULL || tarName == NULL) {
        return false;
    }

    // 查询好友表，判断是否已是好友
    QString sql = QString(R"(
      select * from friend
      where
      (
        user_id = (select id from user_info where name='%1')
        and
        friend_id = (select id from user_info where name='%2')
      )
      or
      (
        user_id = (select id from user_info where name='%2')
        and
        friend_id = (select id from user_info where name='%1')
      )
    )").arg(curName).arg(tarName);

    QSqlQuery q = executeQuery(sql);
    if (!q.next()) {
        return false;
    }

    sql = QString(R"(
      delete from friend
      where
      (
        user_id = (select id from user_info where name='%1')
        and
        friend_id = (select id from user_info where name='%2')
      )
      or
      (
        user_id = (select id from user_info where name='%2')
        and
        friend_id = (select id from user_info where name='%1')
      )
    )").arg(curName).arg(tarName);

    return executeUpdate(sql);
}


// 秒传相关方法实现
bool OperateDB::fileHashExists(const QString &md5)
{
    QString sql = QString("SELECT COUNT(*) FROM file_hash WHERE md5 = '%1'").arg(md5);
    QSqlQuery q = executeQuery(sql);
    if (q.next()) {
        return q.value(0).toInt() > 0;
    }
    return false;
}

bool OperateDB::addFileHash(const QString &md5, const QString &filePath, qint64 fileSize, const QString &fileName)
{
    QString sql = QString("INSERT INTO file_hash (md5, file_path, file_size, file_name) VALUES ('%1', '%2', %3, '%4')")
                  .arg(md5).arg(filePath).arg(fileSize).arg(fileName);
    return executeUpdate(sql);
}

QString OperateDB::getFilePathByHash(const QString &md5)
{
    QString sql = QString("SELECT file_path FROM file_hash WHERE md5 = '%1'").arg(md5);
    QSqlQuery q = executeQuery(sql);
    if (q.next()) {
        return q.value(0).toString();
    }
    return "";
}

bool OperateDB::createFileLink(const QString &md5, const QString &targetPath, const QString &fileName)
{
    // 首先检查文件哈希是否存在
    if (!fileHashExists(md5)) {
        qDebug() << "文件哈希不存在：" << md5;
        return false;
    }

    // 获取源文件路径
    QString sourcePath = getFilePathByHash(md5);
    if (sourcePath.isEmpty()) {
        qDebug() << "无法获取文件路径：" << md5;
        return false;
    }

    // 构建目标文件路径
    QString targetFilePath = targetPath + "/" + fileName;

    // 检查目标文件是否已存在
    QFileInfo targetInfo(targetFilePath);
    if (targetInfo.exists()) {
        qDebug() << "目标文件已存在：" << targetFilePath;
        return true; // 如果文件已存在，也视为成功
    }

    // 创建目录（如果不存在）
    QDir targetDir(targetPath);
    if (!targetDir.exists() && !targetDir.mkpath(targetPath)) {
        qDebug() << "无法创建目标目录：" << targetPath;
        return false;
    }

    // 复制文件
    bool success = QFile::copy(sourcePath, targetFilePath);
    if (success) {
        qDebug() << "文件链接创建成功：" << sourcePath << " -> " << targetFilePath;
    } else {
        qDebug() << "文件链接创建失败：" << sourcePath << " -> " << targetFilePath;
    }

    return success;
}


