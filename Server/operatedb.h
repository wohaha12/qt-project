#ifndef OPERATEDB_H
#define OPERATEDB_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "databaseconnectionpool.h"  // 添加数据库连接池头文件

class OperateDB : public QObject
{
    Q_OBJECT
public:
    ~OperateDB();
    static OperateDB& getInstance();

    void connect();

    // 事务相关方法
    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();

    // 注册功能：返回注册是否成功
    bool handleRegist(const char* caName, const char* caPwd);
    // 登录功能：返回登录是否成功
    bool handleLogin(const char* caName, const char* caPwd);
    // 离线处理
    void handleOfflint(const char* caName);
    // 查找用户
    int handleFindUser(const char* caName);
    // 在线用户
    QStringList handleOnlineUser();
    // 处理添加好友
    int handleAddFriend(const char* curName,const char* tarName);
    // 是否成功添加好友
    bool handleAgreeAddFriend(const char* curName,const char* tarName);
    QStringList handleFlushOnlineUser(const char* caName);
    bool handleDelFriend(const char* curName,const char* tarName);

    // 秒传相关数据库操作方法
    bool fileHashExists(const QString &md5);
    bool addFileHash(const QString &md5, const QString &filePath, qint64 fileSize, const QString &fileName);
    QString getFilePathByHash(const QString &md5);
    bool createFileLink(const QString &md5, const QString &targetPath, const QString &fileName);

private:
    explicit OperateDB(QObject *parent = nullptr);
    OperateDB(const OperateDB& instance) = delete;
    OperateDB& operator=(const OperateDB&) = delete;

    // 当前事务状态和连接
    bool m_inTransaction = false;           
    QSqlDatabase m_currentConnection; 

    // 内部 SQL 操作封装
    QSqlQuery executeQuery(const QString& sql);
    bool executeUpdate(const QString& sql);
};

#endif // OPERATEDB_H
