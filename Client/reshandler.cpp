#include "client.h"
#include "index.h"
#include "reshandler.h"
#include "stdlib.h"

#include <QDir>
#include <qmessagebox.h>
ResHandler::ResHandler()
{

}

void ResHandler::regist()
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof(bool));
    if(ret){
        QMessageBox::information(&Client::getInstance(),"提示","注册成功");
    }else{
        QMessageBox::warning(&Client::getInstance(),"提示","注册失败");
    }
}

void ResHandler::login()
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof(bool));
    if(ret){
        Index::getInstance().show();
        Index::getInstance().setWindowTitle(QString("首页-%1").arg(Client::getInstance().m_strLoginName));
        Client::getInstance().hide();
    }else{
        QMessageBox::warning(&Client::getInstance(),"提示","登录失败");
    }
}

void ResHandler::findUser()
{
    char caName[32] = {'\0'};
    memcpy(caName,pdu->caData,32);
    int ret;
    // 从PDU数据区拷贝返回值
    memcpy(&ret, pdu->caData+32, sizeof(int));
    qDebug()<<"findUser ret"<<ret;
    
    // 检查是否查找的是自己
    QString strCurName = Client::getInstance().m_strLoginName;
    if (QString(caName) == strCurName) {
        QMessageBox::information(&Index::getInstance(), "提示", "不能添加自己为好友");
        return;
    }
    
    // 根据不同的返回值显示相应提示
    if (ret == 2) {
        QMessageBox::information(&Index::getInstance(), "提示", QString("%1已是好友").arg(caName));
    } else if (ret == -1) {
        QMessageBox::information(&Index::getInstance(), "提示", "查找失败");
    } else if (ret == -2) {
        QMessageBox::information(&Index::getInstance(), "提示", "不能添加自己为好友");
    } else if (ret == 0) {
        QMessageBox::information(&Index::getInstance(), "提示", QString("%1不在线").arg(caName));
    } else if (ret == 1) {
        int ret = QMessageBox::information(&Index::getInstance(), "提示", QString("%1在线").arg(caName),"添加好友","取消");
        qDebug()<<"ret"<<ret;
        if(ret == 0){
            PDU* pdu = mkPDU();
            pdu->uiType = ENUM_MSG_TYPE_ADD_FRIEND_REQUEST;
            memcpy(pdu->caData,strCurName.toStdString().c_str(),32);
            memcpy(pdu->caData+32,caName,32);
            Client::getInstance().sendMsg(pdu);
        }
    }
}

void ResHandler::onlineUser(QString& strLoginName)
{
    int iSize = pdu->uiMsgLen/32;
    QStringList res;
    char caName[32] = {'\0'};
    for(int i = 0;i<iSize;i++){
        memcpy(caName,pdu->caMsg+i*32,32);
        if(QString(caName) == strLoginName){
            continue;
        }
        res.append(caName);
    }
    Index::getInstance().getFriend()->m_pOnlineUser->updateUserList(res);
}

void ResHandler::addFriend()
{
    int ret;
    memcpy(&ret,pdu->caData,sizeof(int));
    qDebug()<<"addFriend ret"<<ret;
    if(ret == 2){
        QMessageBox::information(&Index::getInstance(),"提示","该用户已是好友");
    }
    else if(ret == -1){
        QMessageBox::information(&Index::getInstance(),"提示","查询失败");
    }
    else if(ret == 0){
        QMessageBox::information(&Index::getInstance(),"提示","该用户不在线");
    }
}

void ResHandler::addFriendResend()
{
    char curName[32] = {'\0'};
    memcpy(curName,pdu->caData,32);
    int ret = QMessageBox::question(&Index::getInstance(),"提示",QString("%1 请求添加您为好友，是否同意？").arg(curName));
    if(ret != QMessageBox::Yes){
        return;
    }
    PDU* respdu = mkPDU();
    respdu->uiType = ENUM_MSG_TYPE_AGREE_ADD_FRIEND_REQUEST;
    memcpy(respdu->caData,pdu->caData,64);
    Client::getInstance().sendMsg(respdu);
}

void ResHandler::agreeAddFriend()
{
    bool ret;
    memcpy(&ret, pdu->caData, sizeof(bool));
    if (ret) {
        Index::getInstance().getFriend()->flushFriend();
    } else {
        QMessageBox::warning(&Client::getInstance(), "提示", "添加好友失败");
    }
}

void ResHandler::flushFriend()
{
    int iSize = pdu->uiMsgLen/32;
    QStringList res;
    char caName[32] = {'\0'};
    for(int i = 0;i<iSize;i++){
        memcpy(caName,pdu->caMsg+i*32,32);
        res.append(caName);
    }
    Index::getInstance().getFriend()->updateFriendrList(res);
}

void ResHandler::delFriend()
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof(bool));
    qDebug()<<"delFriend ret"<<ret;
    if(ret){
        QMessageBox::information(&Index::getInstance(),"提示","删除成功");
        flushFriend();
    }
    else {
        QMessageBox::information(&Index::getInstance(),"提示","删除失败");
    }
}

void ResHandler::chat()
{
    Chat* c = Index::getInstance().getFriend()->m_pChat;
    if(c->isHidden()){
        c->show();
    }
    char caChatName[32] = {'\0'};
    memcpy(caChatName,pdu->caData,32);
    c->m_strChatName = caChatName;
    c->addMsg(QString("%1 : %2").arg(caChatName).arg(pdu->caMsg));

}

void ResHandler::mkDir()
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof(bool));
    if(ret){
        Index::getInstance().getFile()->flushFile();
    }else{
        QMessageBox::warning(&Index::getInstance(),"提示","创建文件夹失败");
    }
}

void ResHandler::flushFile()
{
    int iCount = pdu->uiMsgLen/sizeof(FileInfo);
    qDebug()<<"iCount"<<iCount;
    QList<FileInfo*> pFileList;
    for(int i = 0;i<iCount;i++){
        FileInfo* pFileInfo = new FileInfo;
        memcpy(pFileInfo,pdu->caMsg+i*sizeof(FileInfo),sizeof(FileInfo));
        pFileList.append(pFileInfo);
    }
    Index::getInstance().getFile()->updateFileList(pFileList);
}

void ResHandler::moveFile()
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof(bool));
    if(ret){
        Index::getInstance().getFile()->flushFile();
    }else{
        QMessageBox::warning(&Index::getInstance(),"提示","移动文件夹失败");
    }
}

//void ResHandler::uploadFile()
//{
//    bool ret;
//    memcpy(&ret,pdu->caData,sizeof(bool));
//    if(ret){
//        Index::getInstance().getFile()->uploadFile();
//    }else{
//        QMessageBox::warning(&Index::getInstance(),"提示","服务器初始化失败");
//    }
//}


// ... 现有代码 ...
void ResHandler::uploadFile()
{
    bool ret;
    memcpy(&ret, pdu->caData, sizeof(bool));
    if(ret){
        // 检查是否是秒传成功（通过额外数据判断）
        bool isQuickUpload = false;
        if (pdu->uiMsgLen > 0) {
            isQuickUpload = *(bool*)(pdu->caMsg);
        }

        if (isQuickUpload) {
            QMessageBox::information(&Index::getInstance(), "提示", "文件秒传成功");
            Index::getInstance().getFile()->flushFile();
        } else {
            // 普通上传流程
            Index::getInstance().getFile()->uploadFile();
        }
    } else {
        QMessageBox::warning(&Index::getInstance(), "提示", "上传文件失败");
    }
}

// 添加处理MD5查询响应的函数
void ResHandler::queryFileMD5Respond()
{
    bool exists = *(bool*)(pdu->caData);
    QString md5 = pdu->caMsg;

    qDebug() << "MD5查询结果: " << md5 << " exists: " << exists;

    // 获取File实例
    File* fileWidget = Index::getInstance().getFile();
    if (!fileWidget) {
        qDebug() << "Error: File widget not found";
        return;
    }

    if (exists) {
        // 文件已存在，秒传处理
        QMessageBox::information(&Index::getInstance(), "提示", "文件已存在，将使用秒传");
        
        // 准备创建秒传文件请求
        QFileInfo fileInfo(fileWidget->m_strUploadFilePath);
        if (!fileInfo.exists()) {
            QMessageBox::warning(&Index::getInstance(), "错误", "本地文件不存在");
            return;
        }
        
        qint64 fileSize = fileInfo.size();
        QString fileName = fileInfo.fileName();

        // 创建秒传请求
        uint msgLen = md5.size() + 1 + sizeof(bool); // 为秒传标记预留空间
        PDU* respdu = mkPDU(msgLen);
        respdu->uiType = ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST; // 复用现有类型
        memset(respdu->caData, 0, 32); // 清空内存
        memcpy(respdu->caData, fileName.toStdString().c_str(), qMin(32, (int)fileName.size()));
        memcpy(respdu->caData + 32, &fileSize, sizeof(qint64));
        memset(respdu->caMsg, 0, msgLen); // 清空内存
        memcpy(respdu->caMsg, md5.toStdString().c_str(), md5.size());
        respdu->caMsg[md5.size()] = '\0';

        // 正确标记为秒传请求
        bool quickUploadFlag = true;
        memcpy(respdu->caMsg + md5.size() + 1, &quickUploadFlag, sizeof(bool));

        Client::getInstance().sendMsg(respdu);
    } else {
        // 文件不存在，启动正常上传流程
        qDebug() << "文件不存在，启动上传流程";
        fileWidget->uploadFile();
    }
}

// 在ResHandler类中添加
void ResHandler::uploadFileSlice()
{
    bool success;
    memcpy(&success, pdu->caData, sizeof(bool));
    if (!success) {
        QMessageBox::warning(&Index::getInstance(), "提示", "分片上传失败");
    }
    // 可以在这里添加进度更新逻辑
}

void ResHandler::uploadFileComplete()
{
    bool success;
    memcpy(&success, pdu->caData, sizeof(bool));
    if (success) {
        QMessageBox::information(&Index::getInstance(), "提示", "文件上传完成");
        Index::getInstance().getFile()->flushFile();
    } else {
        QMessageBox::warning(&Index::getInstance(), "提示", "文件合并失败");
    }
}





void ResHandler::uploadFileData()
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof(bool));
    if(ret){
        Index::getInstance().getFile()->flushFile();
    }else{
        QMessageBox::warning(&Index::getInstance(),"提示","上传文件失败");
    }
}

void ResHandler::shareFile()
{
    QMessageBox::information(Index::getInstance().getFile(),"分享文件","文件已分享");
}

void ResHandler::shareFileRequest()
{
    QString strSharePath = QString(pdu->caMsg);
    int index = strSharePath.lastIndexOf('/');
    QString strFileName = strSharePath.right(strSharePath.size()-index-1);
    QString strMsg = QString("%1 分享文件: %2\n是否接收？").arg(pdu->caData).arg(strFileName);
    int ret = QMessageBox::question(&Index::getInstance(),"分享文件",strMsg);
    if(ret != QMessageBox::Yes){
        return;
    }
    PDU* respdu = mkPDU(pdu->uiMsgLen);
    respdu->uiType = ENUM_MSG_TYPE_SHARE_FILE_AGREE_REQUEST;
    QString strName = Client::getInstance().m_strLoginName;
    memcpy(respdu->caData,strName.toStdString().c_str(),32);
    memcpy(respdu->caMsg,pdu->caMsg,pdu->uiMsgLen);
    Client::getInstance().sendMsg(respdu);
}

void ResHandler::shareFileResult()
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof(bool));
    if(ret){
        Index::getInstance().getFile()->flushFile();
    }else{
        QMessageBox::warning(&Index::getInstance(),"提示","分享文件失败");
    }
}

// 下载文件响应处理
void ResHandler::downloadFile()
{
    if (pdu == NULL) return;
    
    QString strFileName = pdu->caData;
    bool bSuccess = false;
    uint uiFileSize = 0;
    uint uiCurPos = 0;
    
    // 解析服务器响应
    if (pdu->uiMsgLen >= sizeof(bool) + sizeof(uint) + sizeof(uint)) {
        char* pData = (char*)pdu->caMsg;
        memcpy(&bSuccess, pData, sizeof(bool));
        pData += sizeof(bool);
        memcpy(&uiFileSize, pData, sizeof(uint));
        pData += sizeof(uint);
        memcpy(&uiCurPos, pData, sizeof(uint));
    }
    
    if (bSuccess) {
        qDebug() << "下载文件响应成功:" << strFileName;
        qDebug() << "文件大小:" << uiFileSize;
        
        // 发送下载数据请求
        PDU* pRequestPDU = mkPDU(0);
        pRequestPDU->uiType = ENUM_MSG_TYPE_DOWNLOAD_FILE_DATA_REQUEST;
        memcpy(pRequestPDU->caData, strFileName.toStdString().c_str(), qMin(32, (int)strFileName.size()));
        
        Client::getInstance().sendMsg(pRequestPDU);
    } else {
        QMessageBox::warning(NULL, "下载失败", "服务器无法找到该文件或权限不足");
    }
}

// 下载文件数据处理
void ResHandler::downloadFileData()
{
    if (pdu == NULL) return;
    
    bool bLastBlock = false;
    uint uiCurPos = 0;
    uint uiDataLen = 0;
    
    // 解析数据包
    if (pdu->uiMsgLen >= sizeof(bool) + sizeof(uint)) {
        char* pData = (char*)pdu->caMsg;
        memcpy(&bLastBlock, pData, sizeof(bool));
        pData += sizeof(bool);
        memcpy(&uiCurPos, pData, sizeof(uint));
        pData += sizeof(uint);
        
        uiDataLen = pdu->uiMsgLen - (sizeof(bool) + sizeof(uint));
        
        // 获取当前下载文件的保存路径
        File* pFileWidget = File::getInstance();
        if (pFileWidget) {
            QString strSavePath = pFileWidget->m_strDownloadSavePath;
            
            // 写入文件
            QFile file(strSavePath);
            if (file.open(QIODevice::ReadWrite)) {
                file.seek(uiCurPos);
                file.write(pData, uiDataLen);
                file.close();
                
                qDebug() << "写入数据块，位置:" << uiCurPos << "长度:" << uiDataLen;
                
                // 如果不是最后一块，继续请求下一块数据
                if (!bLastBlock) {
                    PDU* pNextRequestPDU = mkPDU(0);
                    pNextRequestPDU->uiType = ENUM_MSG_TYPE_DOWNLOAD_FILE_DATA_REQUEST;
                    memcpy(pNextRequestPDU->caData, pFileWidget->m_strDownloadFileName.toStdString().c_str(), 
                           qMin(32, (int)pFileWidget->m_strDownloadFileName.size()));
                    
                    Client::getInstance().sendMsg(pNextRequestPDU);
                }
            } else {
                qDebug() << "无法打开文件进行写入:" << strSavePath;
            }
        }
    }
}

// 下载完成通知处理
void ResHandler::downloadFileComplete()
{
    if (pdu == NULL) return;
    
    QString strFileName = pdu->caData;
    bool bSuccess = false;
    
    if (pdu->uiMsgLen >= sizeof(bool)) {
        memcpy(&bSuccess, pdu->caMsg, sizeof(bool));
    }
    
    if (bSuccess) {
        File* pFileWidget = File::getInstance();
        if (pFileWidget) {
            QMessageBox::information(NULL, "下载成功", "文件 " + strFileName + " 下载完成");
            qDebug() << "文件下载完成:" << pFileWidget->m_strDownloadSavePath;
        }
    } else {
        QMessageBox::warning(NULL, "下载失败", "文件下载过程中发生错误");
    }
}

// 重命名响应处理函数实现
void ResHandler::renameFileResponse(PDU* pdu)
{
    if(pdu == NULL) {
        qDebug() << "重命名响应PDU为空";
        return;
    }
    
    // 解析响应数据
    bool success = false;
    char* pData = pdu->caMsg;
    memcpy(&success, pData, sizeof(bool));
    pData += sizeof(bool);
    
    // 获取旧文件名和新文件名
    QString oldName = QString::fromLocal8Bit(pData);
    pData += oldName.toLocal8Bit().size() + 1;
    
    QString newName = QString::fromLocal8Bit(pData);
    
    // 根据响应结果进行处理
    if(success) {
        qDebug() << "文件重命名成功：" << oldName << " -> " << newName;
        
        // 显示成功提示
        QMessageBox::information(File::getInstance(), "成功", "文件重命名成功");
        
        // 刷新文件列表以更新显示
        File::getInstance().flushFile();
    } else {
        qDebug() << "文件重命名失败：" << oldName;
        
        // 显示失败提示
        QMessageBox::warning(File::getInstance(), "失败", "文件重命名失败，请稍后重试");
    }
}



