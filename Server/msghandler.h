#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include "protocol.h"
#include <stdlib.h>
#include <QDebug>
#include <qfile.h>
#include <QString>
#include <QMap>

class MsgHandler
{
public:
    MsgHandler();
    PDU* pdu;
    QFile m_fUploadFile;
    qint64 m_iUploadFileSize;
    qint64 m_iReceiveSzie;

    // 添加缺失的成员变量
    QString m_strUploadPath; // 存储上传文件的完整路径

    // 分片上传相关的成员变量
    QMap<QString, QFile*> m_tempFiles; // 存储临时分片文件（已废弃，保留用于兼容）
    QMap<QString, QString> m_uploadPaths; // 存储每个任务的上传路径
    QMap<QString, qint64> m_totalFileSizes; // 存储总文件大小
    QMap<QString, qint64> m_receivedSizes; // 存储已接收的大小
    QMap<QString, int> m_totalSlices; // 存储总分片数
    QMap<QString, QSet<int>> m_receivedSlices; // 存储已接收的分片索引
    
    // 下载相关成员变量
    QString m_strDownloadPath;  // 当前下载文件路径
    qint64 m_iFileSize;        // 文件总大小
    qint64 m_iSentSize;        // 已发送大小
    
private:
    // 计算文件MD5的方法
    QString calculateFileMD5(const QString &filePath);

    PDU* regist();
    PDU* login(QString& strLogiName);
    PDU* findUser();
    PDU* onlineUser();
    PDU* addFriend();
    PDU* agreeAddFriend();
    PDU* flushFriend();
    PDU* delFriend();
    PDU* chat();
    PDU* mkDir();
    PDU* flushFile();
    PDU* moveFile();
    PDU* uploadFile();
    PDU* uploadFileData();
    PDU* shareFlie();
    PDU* shareFlieAgree();
    bool copyDir(QString strSrcDir,QString strDestDir);

    // 新增文件哈希查询处理方法
    PDU *queryFileMD5();

    // 新增分片上传相关方法
    PDU* uploadFileSlice(); // 处理分片上传请求
    PDU* uploadFileComplete(); // 处理上传完成请求
    PDU* downloadFile();
    PDU* downloadFileData();
};

#endif // MSGHANDLER_H
