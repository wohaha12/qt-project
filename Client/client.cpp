#include "client.h"
#include "ui_client.h"
#include "protocol.h"
#include "index.h"
#include "memorypool.h"
#include <QDebug>
#include <QMessageBox>
#include <QAbstractSocket>
Client::Client(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);
    loadConfig();//加载配置文件
    socket.connectToHost(QHostAddress(m_strIP),m_usPort);
    //连接信号与槽函数
    connect(&socket,&QTcpSocket::connected,this,&Client::showConnect);
    connect(&socket, &QTcpSocket::readyRead, this, &Client::recvMsg);
    // 使用旧版本的error信号（兼容Qt 5.0+）
    connect(&socket, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error), 
            this, &Client::handleSocketError);
    connect(&socket, &QTcpSocket::disconnected, this, &Client::handleDisconnected);
    rh = new ResHandler;
}

Client::~Client()
{
    delete ui;
    delete rh;
}
//加载配置文件
void Client::loadConfig()
{
    QFile file(":/client.config");
    if(file.open(QIODevice::ReadOnly)){
        QString strData = QString(file.readAll());
        qDebug() << "strData" << strData;
        QStringList strList = strData.split("\r\n");
        m_strIP = strList[0];
        m_usPort = strList[1].toUShort();
        //m_strLoginName = strList[2];
        m_strRootPath = strList[2];
        qDebug() << "loadConfig ip" << m_strIP << "port" << m_usPort<<"m_strLoginName"<< m_strLoginName;
        file.close();
    }else{
        qDebug() << "loadConfig 打开配置文件失败";
    }
}

//void Client::sendMsg(PDU *pdu)
//{
//    socket.write((char*)pdu,pdu->uiPDULen);
//    qDebug()<<"pdu->uiPDULen"<<pdu->uiPDULen
//            <<"pdu->uiType"<<pdu->uiType
//            <<"pdu->caData"<<pdu->caData
//            <<"pdu->caData+32"<<pdu->caData+32
//            <<"pdu->caMsg"<<pdu->caMsg;
//    free(pdu);
//    pdu = NULL;
//}


void Client::sendMsg(PDU *pdu)
{
    if(pdu == NULL) {
        qWarning() << "sendMsg: pdu is NULL";
        return;
    }
    // 检查socket连接状态
    if(socket.state() != QAbstractSocket::ConnectedState) {
        qWarning() << "sendMsg: socket not connected, state:" << socket.state();
        MemoryPool::getInstance().deallocatePDU(pdu);
        return;
    }
    qint64 bytesWritten = socket.write((char*)pdu, pdu->uiPDULen);
    if(bytesWritten != pdu->uiPDULen) {
        qWarning() << "sendMsg: 部分数据写入失败，期望:" << pdu->uiPDULen << "实际:" << bytesWritten;
    }
    qDebug()<<"pdu->uiPDULen"<<pdu->uiPDULen
            <<"pdu->uiType"<<pdu->uiType
            <<"pdu->caData"<<pdu->caData
            <<"pdu->caData+32"<<pdu->caData+32
            <<"pdu->caMsg"<<pdu->caMsg;
    // 使用内存池释放PDU，而不是直接free
    MemoryPool::getInstance().deallocatePDU(pdu);
    pdu = NULL;
}




void Client::handlerMsg(PDU *pdu)
{
    if(pdu == NULL){
        return;
    }
    rh->pdu = pdu;
    switch(pdu->uiType){
    //注册
    case ENUM_MSG_TYPE_REGIST_RESPOND:{
        rh->regist();
        break;
    }
    //登录
    case ENUM_MSG_TYPE_LOGIN_RESPOND:{
        rh->login();
        break;
    }
    // 处理查找用户响应
    case ENUM_MSG_TYPE_FIND_USER_RESPOND:{
        rh->findUser();
        break;
    }
    // 在线用户
    case ENUM_MSG_TYPE_ONLINE_USER_RESPOND:{
        rh->onlineUser(m_strLoginName);
        break;
    }
    // 添加用户
    case ENUM_MSG_TYPE_ADD_FRIEND_RESPOND:{
        rh->addFriend();
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST:{
        rh->addFriendResend();
        break;
    }
    case ENUM_MSG_TYPE_AGREE_ADD_FRIEND_RESPOND:{
        rh->agreeAddFriend();
        break;
    }
    //刷新好友
    case ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND:{
        rh->flushFriend();
        break;
    }
    case ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND:{
        rh->delFriend();
        break;
    }
    case ENUM_MSG_TYPE_CHAT_RESPOND:{
        rh->chat();
        break;
    }
    case ENUM_MSG_TYPE_MKDIR_RESPOND:{
        rh->mkDir();
        break;
    }
    case ENUM_MSG_TYPE_FLUSH_FILE_RESPOND:{
        rh->flushFile();
        break;
    }
    case ENUM_MSG_TYPE_MOVE_FILE_RESPOND:{
        rh->moveFile();
        break;
    } 
    case ENUM_MSG_TYPE_UPLOAD_FILE_RESPOND:{
        rh->uploadFile();
        break;
    }
    case ENUM_MSG_TYPE_UPLOAD_FILE_DATA_RESPOND:{
        rh->uploadFileData();
        break;
    }
    case ENUM_MSG_TYPE_SHARE_FILE_RESPOND:{
        rh->shareFile();
        break;
    }
    case ENUM_MSG_TYPE_SHARE_FILE_REQUEST:{
        rh->shareFileRequest();
        break;
    }
    case ENUM_MSG_TYPE_SHARE_FILE_AGREE_RESPOND:{
        rh->shareFileResult();
        break;
    }




   // 添加MD5查询响应的处理
    case ENUM_MSG_TYPE_QUERY_FILE_MD5_RESPOND:{
         rh->queryFileMD5Respond();
         break;
    }
        // 在handlerMsg函数的switch语句中添加
    case ENUM_MSG_TYPE_UPLOAD_FILE_SLICE_RESPOND:
        rh->uploadFileSlice();
        break;
    case ENUM_MSG_TYPE_UPLOAD_FILE_COMPLETE_RESPOND:
        rh->uploadFileComplete();
        break;
        
    // 下载相关消息处理
    case ENUM_MSG_TYPE_DOWNLOAD_FILE_RESPOND:
        rh->downloadFile(pdu);
        break;
    case ENUM_MSG_TYPE_DOWNLOAD_FILE_DATA:
        rh->downloadFileData(pdu);
        break;
    case ENUM_MSG_TYPE_DOWNLOAD_FILE_COMPLETE:
        rh->downloadFileComplete(pdu);
        break;
        
    // 重命名响应处理
    case ENUM_MSG_TYPE_RENAME_FILE_RESPOND:
        rh->renameFileResponse(pdu);
        break;

    default:
        break;
    }
}

void Client::showConnect()
{
    qDebug() <<"TCP连接服务器成功";
}

void Client::handleSocketError(QAbstractSocket::SocketError error)
{
    QString errorMsg;
    switch(error) {
    case QAbstractSocket::ConnectionRefusedError:
        errorMsg = "连接被拒绝，请检查服务器是否运行";
        break;
    case QAbstractSocket::RemoteHostClosedError:
        errorMsg = "远程主机关闭了连接";
        break;
    case QAbstractSocket::HostNotFoundError:
        errorMsg = "找不到主机，请检查IP地址";
        break;
    case QAbstractSocket::NetworkError:
        errorMsg = "网络错误，请检查网络连接";
        break;
    case QAbstractSocket::SocketTimeoutError:
        errorMsg = "连接超时";
        break;
    default:
        errorMsg = QString("连接错误: %1").arg(socket.errorString());
        break;
    }
    qDebug() << "Socket错误:" << errorMsg;
    QMessageBox::warning(this, "连接错误", errorMsg);
}

void Client::handleDisconnected()
{
    qDebug() << "与服务器断开连接";
    QMessageBox::information(this, "提示", "与服务器断开连接");
}

Client& Client::getInstance(){
    static Client instance;
    return instance;
}

void Client::on_regist_PB_clicked()
{
    QString strName = ui->name_LE->text();
    QString strPwd = ui->pwd_LE->text();
    if(strName.isEmpty() || strPwd.isEmpty()){
        QMessageBox::information(this,"提示","用户名或密码不能为空");
        return;
    }
    if(strName.size()>32 || strPwd.size()>32){
        QMessageBox::information(this,"提示","用户名或密码长度不合法");
        return;
    }
    // 检查socket连接状态
    if(socket.state() != QAbstractSocket::ConnectedState) {
        QMessageBox::warning(this, "提示", "未连接到服务器，请先连接服务器");
        return;
    }
    PDU* pdu = mkPDU();
    if(pdu == NULL) {
        QMessageBox::critical(this, "错误", "内存分配失败");
        return;
    }
    memcpy(pdu->caData,strName.toStdString().c_str(),32);
    memcpy(pdu->caData+32,strPwd.toStdString().c_str(),32);
    pdu->uiType = ENUM_MSG_TYPE_REGIST_REQUEST;
    sendMsg(pdu);
}

//void Client::recvMsg()
//{
//    qDebug()<<"recvMsg接收消息长度"<<socket.bytesAvailable();
//    //定义buffer成员变量，全部未处理的数据
//    QByteArray data = socket.readAll();
//    buffer.append(data);
//    while(buffer.size()>=int(sizeof(PDU))){
//        PDU* pdu = (PDU*)buffer.data();
//        if(buffer.size()<int(pdu->uiPDULen)){
//            break;
//        }
//        handlerMsg(pdu);
//        buffer.remove(0,pdu->uiPDULen);
//    }
//}



void Client::recvMsg()
{
    qDebug()<<"recvMsg接收消息长度"<<socket.bytesAvailable();
    //定义buffer成员变量，全部未处理的数据
    QByteArray data = socket.readAll();
    buffer.append(data);
    while(buffer.size()>=int(sizeof(PDU))){
        PDU* pdu = (PDU*)buffer.data();
        if(buffer.size()<int(pdu->uiPDULen)){
            break;
        }
        // 创建一个临时PDU用于处理
        PDU* tempPdu = MemoryPool::getInstance().allocatePDU(pdu->uiMsgLen);
        if(tempPdu == NULL) {
            qCritical() << "recvMsg: 内存分配失败，无法处理消息";
            break;
        }
        memcpy(tempPdu, pdu, pdu->uiPDULen);
        uint uiPDULen = pdu->uiPDULen; // 保存长度，因为pdu在buffer.remove后可能失效
        handlerMsg(tempPdu);
        // 处理完成后释放临时PDU
        MemoryPool::getInstance().deallocatePDU(tempPdu);
        buffer.remove(0, uiPDULen);
    }
}








void Client::on_login_PB_clicked()
{
    QString strName = ui->name_LE->text();
    QString strPwd = ui->pwd_LE->text();
    if(strName.isEmpty() || strPwd.isEmpty()){
        QMessageBox::information(this,"提示","用户名或密码不能为空");
        return;
    }
    if(strName.size()>32 || strPwd.size()>32){
        QMessageBox::information(this,"提示","用户名或密码长度不合法");
        return;
    }
    // 检查socket连接状态
    if(socket.state() != QAbstractSocket::ConnectedState) {
        QMessageBox::warning(this, "提示", "未连接到服务器，请先连接服务器");
        return;
    }
    PDU* pdu = mkPDU();
    if(pdu == NULL) {
        QMessageBox::critical(this, "错误", "内存分配失败");
        return;
    }
    memcpy(pdu->caData,strName.toStdString().c_str(),32);
    memcpy(pdu->caData+32,strPwd.toStdString().c_str(),32);
    m_strLoginName = strName;
    pdu->uiType = ENUM_MSG_TYPE_LOGIN_REQUEST;
    sendMsg(pdu);
    qDebug()<<"发送登录请求"<<strName;
}


