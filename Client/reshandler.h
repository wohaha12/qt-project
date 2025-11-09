#ifndef RESHANDLER_H
#define RESHANDLER_H

#include "protocol.h"

#include <QString>



class ResHandler
{
public:
    ResHandler();
    PDU* pdu;
    void regist();
    void login();
    void findUser();
    void onlineUser(QString& strLoginName);
    void addFriend();
    void addFriendResend();
    void agreeAddFriend();
    void flushFriend();
    void delFriend();
    void chat();
    void mkDir();
    void flushFile();
    void moveFile();
    void uploadFile();
    void uploadFileData();
    void shareFile();
    void shareFileRequest();
    void shareFileResult();



    void queryFileMD5Respond(); // 添加MD5查询响应处理函数

    // 在class ResHandler的声明中添加
    void uploadFileSlice();
    void uploadFileComplete();
};

#endif // RESHANDLER_H
