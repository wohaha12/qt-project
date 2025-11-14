#include "mytcpsocket.h"
#include <QDebug>
#include <QTcpServer>
#include <QAbstractSocket>
#include "operatedb.h"
#include "mytcpserver.h"
#include "msghandler.h"
#include "memorypool.h" // 添加头文件包含

PDU *MyTcpSocket::handleMsg(PDU *pdu)
{
    if(pdu == NULL){
        return NULL;
    }
    qDebug() << "handleMsg pdu->uiPDULen" <<pdu->uiPDULen
             << "pdu->uiMsgLen" <<pdu->uiMsgLen
             << "pdu->uiType" <<pdu->uiType
             << "pdu->caData" <<pdu->caData
             << "pdu->caData + 32" <<pdu->caData + 32
             << "pdu->caMsg" <<pdu->caMsg;
    mh->pdu = pdu;
    switch(pdu->uiType){
    //注册
    case ENUM_MSG_TYPE_REGIST_REQUEST:
        return mh->regist();
    //登录
    case ENUM_MSG_TYPE_LOGIN_REQUEST:
        return mh->login(m_strLoginName);
    //查找用户
    case ENUM_MSG_TYPE_FIND_USER_REQUEST:
        return mh->findUser();
    //在线用户
    case ENUM_MSG_TYPE_ONLINE_USER_REQUEST:
        return mh->onlineUser();
    //添加好友
    case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST:
        return mh->addFriend();
    //是否添加好友
    case ENUM_MSG_TYPE_AGREE_ADD_FRIEND_REQUEST:
        return mh->agreeAddFriend();
    case ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST:
        return mh->flushFriend();
    case ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST:
        return mh->delFriend();
    case ENUM_MSG_TYPE_CHAT_REQUEST:
        return mh->chat();
    case ENUM_MSG_TYPE_MKDIR_REQUEST:
        return mh->mkDir();
    case ENUM_MSG_TYPE_FLUSH_FILE_REQUEST:
        return mh->flushFile();
    case ENUM_MSG_TYPE_MOVE_FILE_REQUEST:
        return mh->moveFile();
    case ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST:
        return mh->uploadFile();
    case ENUM_MSG_TYPE_UPLOAD_FILE_DATA_REQUEST:
        return mh->uploadFileData();
    case ENUM_MSG_TYPE_SHARE_FILE_REQUEST:
        return mh->shareFlie();
    case ENUM_MSG_TYPE_SHARE_FILE_AGREE_REQUEST:
        return mh->shareFlieAgree();
    // 查询文件MD5
    case ENUM_MSG_TYPE_QUERY_FILE_MD5_REQUEST:
        return mh->queryFileMD5();
    // 分片上传相关消息类型
    case ENUM_MSG_TYPE_UPLOAD_FILE_SLICE_REQUEST:
        return mh->uploadFileSlice();
    case ENUM_MSG_TYPE_UPLOAD_FILE_COMPLETE_REQUEST:
        return mh->uploadFileComplete();
    // 下载相关消息类型
    case ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST:
        return mh->downloadFile();
    case ENUM_MSG_TYPE_DOWNLOAD_FILE_DATA_REQUEST:
        return mh->downloadFileData();

    default:
        return NULL;
    }
}

//void MyTcpSocket::sendMsg(PDU *pdu)
//{
//    if(pdu == NULL){
//        return;
//    }
//    this->write((char*)pdu,pdu->uiPDULen);
//    qDebug() << "sendMsg pdu->uiPDULen" <<pdu->uiPDULen
//             << "pdu->uiMsgLen" <<pdu->uiMsgLen
//             << "pdu->uiType" <<pdu->uiType
//             << "pdu->caData" <<pdu->caData
//             << "pdu->caData + 32" <<pdu->caData + 32
//             << "pdu->caMsg" <<pdu->caMsg;
//    free(pdu);
//    pdu = NULL;
//}

void MyTcpSocket::sendMsg(PDU *pdu)
{
    if(pdu == NULL){
        qWarning() << "sendMsg: pdu is NULL";
        return;
    }
    // 检查socket状态
    if(this->state() != QAbstractSocket::ConnectedState) {
        qWarning() << "sendMsg: socket not connected, state:" << this->state();
        MemoryPool::getInstance().deallocatePDU(pdu);
        return;
    }
    qint64 bytesWritten = this->write((char*)pdu, pdu->uiPDULen);
    if(bytesWritten != pdu->uiPDULen) {
        qWarning() << "sendMsg: 部分数据写入失败，期望:" << pdu->uiPDULen << "实际:" << bytesWritten;
    }
    qDebug() << "sendMsg pdu->uiPDULen" <<pdu->uiPDULen
             << "pdu->uiMsgLen" <<pdu->uiMsgLen
             << "pdu->uiType" <<pdu->uiType
             << "pdu->caData" <<pdu->caData
             << "pdu->caData + 32" <<pdu->caData + 32
             << "pdu->caMsg" <<pdu->caMsg;
    // 使用内存池释放PDU对象
    MemoryPool::getInstance().deallocatePDU(pdu);
    pdu = NULL;
}



MyTcpSocket::~MyTcpSocket()
{
    delete mh;
}


// 在文件开头添加构造函数的实现
MyTcpSocket::MyTcpSocket()
{
    // 初始化MsgHandler指针
    mh = new MsgHandler;

    // 连接信号和槽
    connect(this, &QTcpSocket::readyRead, this, &MyTcpSocket::recvMsg);
    connect(this, &QTcpSocket::disconnected, this, &MyTcpSocket::clientOffline);
}




//void MyTcpSocket::recvMsg()
//{
//    qDebug()<<"recvMsg接收消息长度"<<this->bytesAvailable();
//    //定义buffer成员变量，全部未处理的数据
//    QByteArray data = this->readAll();
//    buffer.append(data);
//    while(buffer.size()>=int(sizeof(PDU))){
//        PDU* pdu = (PDU*)buffer.data();
//        if(buffer.size()<int(pdu->uiPDULen)){
//            break;
//        }
//        PDU* respdu = handleMsg(pdu);
//        sendMsg(respdu);
//        buffer.remove(0,pdu->uiPDULen);
//    }
//}


void MyTcpSocket::recvMsg()
{
    qDebug()<<"recvMsg接收消息长度"<<this->bytesAvailable();
    //定义buffer成员变量，全部未处理的数据
    QByteArray data = this->readAll();
    buffer.append(data);
    while(buffer.size()>=int(sizeof(PDU)))
    {
        PDU* tempPdu = (PDU*)buffer.data();
        if(buffer.size()<int(tempPdu->uiPDULen))
        {
            break;
        }

        // 保存PDU长度，因为tempPdu在buffer.remove后可能失效
        uint uiPDULen = tempPdu->uiPDULen;
        uint uiMsgLen = tempPdu->uiMsgLen;

        // 使用内存池为PDU分配新的内存
        PDU* pdu = MemoryPool::getInstance().allocatePDU(uiMsgLen);
        if (pdu == NULL) {
            qCritical() << "recvMsg: 内存分配失败，无法处理消息";
            break;
        }
        // 复制PDU数据
        memcpy(pdu, tempPdu, uiPDULen);

        // 处理消息并发送响应
        PDU* respdu = handleMsg(pdu);
        sendMsg(respdu);
        
        // 释放从内存池分配的PDU对象
        MemoryPool::getInstance().deallocatePDU(pdu);

        // 从buffer中移除已处理的PDU数据（必须在循环内）
        buffer.remove(0, uiPDULen);
    }
}




void MyTcpSocket::clientOffline()
{
    OperateDB::getInstance().handleOfflint(m_strLoginName.toStdString().c_str());
    MyTcpServer::getInstance().deleteSocket(this);
}
