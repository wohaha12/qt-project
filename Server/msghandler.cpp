#include "msghandler.h"
#include "operatedb.h"
#include "mytcpsocket.h"
#include "mytcpserver.h"
#include "server.h"
#include "memorypool.h"

#include <QDir>
#include <QFileInfo>
#include <QCryptographicHash>
MsgHandler::MsgHandler()
{

}

// 计算文件MD5的方法
QString MsgHandler::calculateFileMD5(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "无法打开文件进行MD5计算:" << filePath;
        return "";
    }

    QCryptographicHash hash(QCryptographicHash::Md5);
    while (!file.atEnd()) {
        hash.addData(file.read(8192)); // 分块读取，避免内存占用过大
    }
    file.close();
    return hash.result().toHex();
}

PDU *MsgHandler::regist()
{
    char caName[32] = {'\0'};
    memcpy(caName,pdu->caData,32);
    char caPwd[32] = {'\0'};
    memcpy(caPwd,pdu->caData+32,32);
    bool ret = OperateDB::getInstance().handleRegist(caName,caPwd);
    qDebug()<<"ret"<<ret;
    if(ret){
        QDir dir;
        bool mkRet = dir.mkdir(QString("%1/%2").arg(Server::getInstance().m_strRootPath).arg(caName));
        qDebug()<<"mkdir ret"<<mkRet;
    }
    PDU* respdu = mkPDU();
    memcpy(respdu->caData,&ret,sizeof(bool));
    respdu->uiType = ENUM_MSG_TYPE_REGIST_RESPOND;
    return respdu;
}

PDU *MsgHandler::login(QString& strLogiName)
{
    char caName[32] = {'\0'};
    char caPwd[32] = {'\0'};
    memcpy(caName,pdu->caData,32);
    memcpy(caPwd,pdu->caData+32,32);
    bool ret = OperateDB::getInstance().handleLogin(caName,caPwd);
    qDebug()<<"ret"<<ret;
    if(ret){
        strLogiName = caName;
    }
    PDU* respdu = mkPDU();
    memcpy(respdu->caData,&ret,sizeof(bool));
    respdu->uiType = ENUM_MSG_TYPE_LOGIN_RESPOND;
    return respdu;
}

PDU *MsgHandler::findUser()
{
    char caName[32] = {'\0'};
    memcpy(caName,pdu->caData,32);
    int ret = OperateDB::getInstance().handleFindUser(caName);
    qDebug() << "查找用户" << caName << "结果：" << ret;
    PDU* respdu = mkPDU();
    memcpy(respdu->caData,caName,32);
    memcpy(respdu->caData+32,&ret,sizeof(int));
    respdu->uiType = ENUM_MSG_TYPE_FIND_USER_RESPOND;
    return respdu;
}

PDU *MsgHandler::onlineUser()
{
    QStringList res = OperateDB::getInstance().handleOnlineUser();
    qDebug() << "res"<<res.size();
    PDU* respdu = mkPDU(res.size()*32);
    for(int i = 0;i<res.size();i++){
        memcpy(respdu->caMsg+i*32,res[i].toStdString().c_str(),32);
    }
    respdu->uiType = ENUM_MSG_TYPE_ONLINE_USER_RESPOND;
    return respdu;
}

PDU* MsgHandler::addFriend()
{
    char curName[32] = {'\0'};
    memcpy(curName,pdu->caData,32);
    char tarName[32] = {'\0'};
    memcpy(tarName,pdu->caData+32,32);
    int ret = OperateDB::getInstance().handleAddFriend(curName,tarName);
    qDebug()<<"addFriend ret"<<ret;
    if(ret == 1){
        MyTcpServer::getInstance().resend(tarName,pdu);
    }
    PDU* respdu = mkPDU();
    memcpy(respdu->caData,&ret,sizeof(ret));
    respdu->uiType = ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
    return respdu;
}

PDU *MsgHandler::agreeAddFriend()
{
    char curName[32] = {'\0'};
    memcpy(curName,pdu->caData,32);
    char tarName[32] = {'\0'};
    memcpy(tarName,pdu->caData+32,32);
    bool ret = OperateDB::getInstance().handleAgreeAddFriend(curName,tarName);
    qDebug()<<"agreeAddFriend ret"<<ret;
    PDU* respdu = mkPDU();
    memcpy(respdu->caData,&ret,sizeof(ret));
    respdu->uiType = ENUM_MSG_TYPE_AGREE_ADD_FRIEND_RESPOND;
    MyTcpServer::getInstance().resend(curName,respdu);
    return respdu;
}

PDU *MsgHandler::flushFriend()
{
    char caName[32] = {'\0'};
    memcpy(caName,pdu->caData,32);
    QStringList res = OperateDB::getInstance().handleFlushOnlineUser(caName);
    qDebug() << "res"<<res.size();
    PDU* respdu = mkPDU(res.size()*32);
    for(int i = 0;i<res.size();i++){
        memcpy(respdu->caMsg+i*32,res[i].toStdString().c_str(),32);
    }
    respdu->uiType = ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND;
    return respdu;
}

PDU *MsgHandler::delFriend()
{
    char curName[32] = {'\0'};
    memcpy(curName,pdu->caData,32);
    char tarName[32] = {'\0'};
    memcpy(tarName,pdu->caData+32,32);
    bool ret = OperateDB::getInstance().handleDelFriend(curName,tarName);
    qDebug()<<"delFriend ret"<<ret;
    PDU* respdu = mkPDU();
    memcpy(respdu->caData,&ret,sizeof(ret));
    respdu->uiType = ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND;
    return respdu;
}

PDU *MsgHandler::chat()
{
    char tarName[32] = {'\0'};
    memcpy(tarName,pdu->caData+32,32);
    
    // 创建响应PDU的副本用于转发
    PDU* resendPdu = MemoryPool::getInstance().allocatePDU(pdu->uiMsgLen);
    if (resendPdu) {
        memcpy(resendPdu, pdu, pdu->uiPDULen);
        resendPdu->uiType = ENUM_MSG_TYPE_CHAT_RESPOND;
        MyTcpServer::getInstance().resend(tarName, resendPdu, true); // 自动释放
    }
    
    // 返回NULL表示不需要发送响应给发送者
    return NULL;
}

PDU *MsgHandler::mkDir()
{
    QDir dir;
    QString strPath = QString("%1/%2").arg(pdu->caMsg).arg(pdu->caData);
    qDebug()<<"mkdir path"<<strPath;
    bool ret;
    if(dir.exists(strPath)){
        ret = false;
        qDebug()<<"文件夹名已存在";
    }
    ret = dir.mkdir(QString("%1/%2").arg(pdu->caMsg).arg(pdu->caData));
    PDU* respdu = mkPDU();
    memcpy(respdu->caData,&ret,sizeof(ret));
    respdu->uiType = ENUM_MSG_TYPE_MKDIR_RESPOND;
    return respdu;
}

PDU *MsgHandler::flushFile()
{
    qDebug()<<"flushFile path"<<pdu->caMsg;
    QDir dir(pdu->caMsg);
    QFileInfoList fileInfoList = dir.entryInfoList();
    QList<QFileInfo> validFiles;
    foreach(QFileInfo info, fileInfoList){
        QString fileName = info.fileName();
        if(fileName != "." && fileName != ".."){
            validFiles.append(info);
        }
    }
    int iFileCount = validFiles.size();
    PDU* respdu = mkPDU(iFileCount*sizeof(FileInfo));
    respdu->uiType = ENUM_MSG_TYPE_FLUSH_FILE_RESPOND;
    for(int i = 0;i<iFileCount;i++){
        FileInfo* pFileInfo = (FileInfo*)respdu->caMsg+i;
        memcpy(pFileInfo->caName,validFiles[i].fileName().toStdString().c_str(),32);
        if(validFiles[i].isDir()){
            pFileInfo->iType = 0;
        }
        else{
            pFileInfo->iType = 1;
        }
        qDebug()<<"pFileInfo->caName"<<pFileInfo->caName;
    }
    return respdu;
}

PDU *MsgHandler::moveFile()
{
    int iSrcPathLen = 0;
    int iTarPathLen = 0;

    memcpy(&iSrcPathLen,pdu->caData,sizeof(int));
    memcpy(&iTarPathLen,pdu->caData+sizeof(int),sizeof(int));

    char* strSrcPath = new char[iSrcPathLen + 1];
    memset(strSrcPath, '\0', iSrcPathLen + 1);

    char* strTarPath = new char[iTarPathLen + 1];
    memset(strTarPath, '\0', iTarPathLen + 1);

    memcpy(strSrcPath,pdu->caMsg,iSrcPathLen);
    memcpy(strTarPath,pdu->caMsg+iSrcPathLen,iTarPathLen);

    qDebug()<<"strSrcPath"<< strSrcPath <<"strTarPath"<< strTarPath;

    bool ret = QFile::rename(strSrcPath,strTarPath);
    PDU* respdu = mkPDU();
    memcpy(respdu->caData,&ret,sizeof(ret));
    respdu->uiType = ENUM_MSG_TYPE_MOVE_FILE_RESPOND;
    delete [] strSrcPath;
    delete [] strTarPath;
    strSrcPath = NULL;
    strTarPath = NULL;
    return respdu;
}

//PDU *MsgHandler::uploadFile()
//{
//    m_iReceiveSzie = 0;
//    char caFileName[32] = {'\0'};
//    memcpy(caFileName,pdu->caData,32);
//    memcpy(&m_iUploadFileSize,pdu->caData+32,sizeof(qint64));
//    QString strPath = QString("%1/%2").arg(pdu->caMsg).arg(caFileName);
//    m_fUploadFile.setFileName(strPath);
//    bool ret = m_fUploadFile.open(QIODevice::WriteOnly);
//    PDU* respdu = mkPDU();
//    memcpy(respdu->caData,&ret,sizeof(ret));
//    respdu->uiType = ENUM_MSG_TYPE_UPLOAD_FILE_RESPOND;
//    return respdu;
//}


PDU *MsgHandler::uploadFile()
{
    m_iReceiveSzie = 0;
    char caFileName[32] = {'\0'};
    memcpy(caFileName, pdu->caData, 32);
    memcpy(&m_iUploadFileSize, pdu->caData + 32, sizeof(qint64));

    // 解析上传路径和MD5
    // caMsg格式: [路径][\0][MD5] 或 [路径][MD5]
    QString strPath;
    QString md5;
    
    // 查找路径和MD5的分隔符
    int pathEnd = 0;
    for (int i = 0; i < pdu->uiMsgLen && i < 1024; i++) { // 限制搜索范围
        if (pdu->caMsg[i] == '\0') {
            pathEnd = i;
            break;
        }
    }
    
    if (pathEnd > 0) {
        // 有分隔符，路径在\0之前
        char* pathData = new char[pathEnd + 1];
        memcpy(pathData, pdu->caMsg, pathEnd);
        pathData[pathEnd] = '\0';
        strPath = QString(pathData);
        delete[] pathData;
        
        // MD5在\0之后
        if (pdu->uiMsgLen > pathEnd + 32) {
            char* md5Data = new char[33];
            memcpy(md5Data, pdu->caMsg + pathEnd + 1, 32);
            md5Data[32] = '\0';
            md5 = QString(md5Data);
            delete[] md5Data;
        }
    } else {
        // 没有分隔符，尝试从末尾提取MD5（32字节）
        if (pdu->uiMsgLen >= 32) {
            int pathLen = pdu->uiMsgLen - 32;
            char* pathData = new char[pathLen + 1];
            memcpy(pathData, pdu->caMsg, pathLen);
            pathData[pathLen] = '\0';
            strPath = QString(pathData);
            delete[] pathData;
            
            char* md5Data = new char[33];
            memcpy(md5Data, pdu->caMsg + pathLen, 32);
            md5Data[32] = '\0';
            md5 = QString(md5Data);
            delete[] md5Data;
        } else {
            // 只有路径，没有MD5
            char* pathData = new char[pdu->uiMsgLen + 1];
            memcpy(pathData, pdu->caMsg, pdu->uiMsgLen);
            pathData[pdu->uiMsgLen] = '\0';
            strPath = QString(pathData);
            delete[] pathData;
        }
    }
    
    // 保存上传路径到成员变量
    m_strUploadPath = QString("%1/%2").arg(strPath).arg(caFileName);
    qDebug() << "上传路径：" << m_strUploadPath << "MD5：" << md5;

    // 检查是否是秒传请求
    bool isQuickUpload = false;
    if (!md5.isEmpty() && md5.size() == 32) {
        // 处理秒传请求
        if (OperateDB::getInstance().fileHashExists(md5)) {
            // 创建文件链接（使用解析后的路径）
            isQuickUpload = OperateDB::getInstance().createFileLink(md5, strPath, caFileName);
            qDebug() << "秒传处理结果：" << isQuickUpload;
        }
    }

    if (!isQuickUpload) {
        // 普通上传流程
        // 确保目录存在
        QDir dir;
        QString dirPath = QFileInfo(m_strUploadPath).absolutePath();
        if (!dir.exists(dirPath)) {
            dir.mkpath(dirPath);
            qDebug() << "创建上传目录：" << dirPath;
        }
        
        // 分片上传初始化：只创建目录，不打开文件
        qDebug() << "初始化分片上传环境，路径：" << m_strUploadPath;
        
        // 为分片上传做准备，保存文件路径信息
        bool ret = true;

        PDU* respdu = mkPDU(0);
        memcpy(respdu->caData, &ret, sizeof(ret));
        respdu->uiType = ENUM_MSG_TYPE_UPLOAD_FILE_RESPOND;
        return respdu;
    } else {
        // 秒传成功的响应
        PDU* respdu = mkPDU(sizeof(bool));
        bool ret = true;
        memcpy(respdu->caData, &ret, sizeof(ret));
        memcpy(respdu->caMsg, &isQuickUpload, sizeof(bool));
        respdu->uiType = ENUM_MSG_TYPE_UPLOAD_FILE_RESPOND;
        return respdu;
    }
}




// 实现文件哈希查询方法
PDU *MsgHandler::queryFileMD5()
{
    // 从pdu中获取MD5值
    QString md5 = pdu->caMsg;
    qDebug() << "查询文件MD5：" << md5;

    // 检查文件哈希是否存在
    bool exists = OperateDB::getInstance().fileHashExists(md5);

    // 创建响应PDU
    PDU* respdu = mkPDU(md5.size() + 1);
    memcpy(respdu->caData, &exists, sizeof(bool));
    memcpy(respdu->caMsg, md5.toStdString().c_str(), md5.size() + 1);
    respdu->uiType = ENUM_MSG_TYPE_QUERY_FILE_MD5_RESPOND;

    return respdu;
}



PDU *MsgHandler::uploadFileData()
{
    m_fUploadFile.write(pdu->caMsg,pdu->uiMsgLen);
    m_iReceiveSzie += pdu->uiMsgLen;
    if(m_iReceiveSzie < m_iUploadFileSize){
        return NULL;
    }
    m_fUploadFile.close();
    PDU* respdu = mkPDU();
    respdu->uiType = ENUM_MSG_TYPE_UPLOAD_FILE_DATA_RESPOND;
    bool ret = m_iReceiveSzie == m_iUploadFileSize;
    memcpy(respdu->caData,&ret,sizeof (ret));
    return respdu;
}

PDU *MsgHandler::shareFlie()
{
    char strCurName[32] = {'\0'};
    int friend_num = 0;
    memcpy(strCurName,pdu->caData,32);
    memcpy(&friend_num,pdu->caData+32,sizeof(int));
    qDebug()<<"strCurName"<<strCurName;
    qDebug()<<"friend_num"<<friend_num;
    int size = friend_num*32;
    PDU* resendpdu = mkPDU(pdu->uiMsgLen - size);
    resendpdu->uiType = pdu->uiType;
    memcpy(resendpdu->caData,strCurName,32);
    memcpy(resendpdu->caMsg,pdu->caMsg+size,pdu->uiMsgLen-size);
    char caRecvName[32] = {'\0'};
    for(int i=0;i<friend_num;i++){
        memcpy(caRecvName,pdu->caMsg+i*32,32);
        qDebug()<<"caRecvName"<<caRecvName;
        MyTcpServer::getInstance().resend(caRecvName,resendpdu, true); // 自动释放
    }
    // resendpdu会在resend函数中自动释放，不需要手动释放
    resendpdu = NULL;
    PDU* respdu = mkPDU();
    respdu->uiType = ENUM_MSG_TYPE_SHARE_FILE_RESPOND;
    return respdu;
}

PDU *MsgHandler::shareFlieAgree()
{
    QString strRecvUserName = QString("%1").arg(pdu->caData);
    QString strShareFilePath = pdu->caMsg;
    
    // 构建接收方的根目录路径
    QString strRecvRootPath = QString("%1/%2").arg(Server::getInstance().m_strRootPath).arg(strRecvUserName);
    
    // 从分享文件路径中提取文件名
    int index = strShareFilePath.lastIndexOf('/');
    QString strFileName = strShareFilePath.right(strShareFilePath.size()-index-1);
    
    // 构建完整的目标文件路径
    QString strRecvPath = strRecvRootPath + '/' + strFileName;
    
    qDebug() << "分享文件路径:" << strShareFilePath;
    qDebug() << "接收方用户名:" << strRecvUserName;
    qDebug() << "接收方根路径:" << strRecvRootPath;
    qDebug() << "目标文件路径:" << strRecvPath;
    
    // 检查源文件是否存在
    QFileInfo fileInfo(strShareFilePath);
    if (!fileInfo.exists()) {
        qDebug() << "分享文件不存在:" << strShareFilePath;
        PDU* respdu = mkPDU();
        bool ret = false;
        memcpy(respdu->caData, &ret, sizeof(bool));
        respdu->uiType = ENUM_MSG_TYPE_SHARE_FILE_AGREE_RESPOND;
        return respdu;
    }
    
    // 确保接收方的根目录存在
    QDir dir;
    if (!dir.exists(strRecvRootPath)) {
        if (!dir.mkpath(strRecvRootPath)) {
            qDebug() << "无法创建接收方目录:" << strRecvRootPath;
            PDU* respdu = mkPDU();
            bool ret = false;
            memcpy(respdu->caData, &ret, sizeof(bool));
            respdu->uiType = ENUM_MSG_TYPE_SHARE_FILE_AGREE_RESPOND;
            return respdu;
        }
    }
    
    bool ret = true;
    if (fileInfo.isFile()) {
        // 检查目标文件是否已存在
        if (QFile::exists(strRecvPath)) {
            // 如果文件已存在，添加时间戳后缀
            QString timestamp = QString::number(QDateTime::currentMSecsSinceEpoch());
            QString baseName = strFileName;
            QString suffix = "";
            
            int dotIndex = strFileName.lastIndexOf('.');
            if (dotIndex != -1) {
                baseName = strFileName.left(dotIndex);
                suffix = strFileName.right(strFileName.size() - dotIndex);
            }
            
            strRecvPath = strRecvRootPath + '/' + baseName + "_" + timestamp + suffix;
            qDebug() << "文件已存在，使用新路径:" << strRecvPath;
        }
        
        ret = QFile::copy(strShareFilePath, strRecvPath);
        if (ret) {
            qDebug() << "文件复制成功:" << strRecvPath;
        } else {
            qDebug() << "文件复制失败:" << strShareFilePath << "到" << strRecvPath;
        }
    } else if (fileInfo.isDir()) {
        // 检查目标目录是否已存在
        if (dir.exists(strRecvPath)) {
            // 如果目录已存在，添加时间戳后缀
            QString timestamp = QString::number(QDateTime::currentMSecsSinceEpoch());
            strRecvPath = strRecvRootPath + '/' + strFileName + "_" + timestamp;
            qDebug() << "目录已存在，使用新路径:" << strRecvPath;
        }
        
        ret = copyDir(strShareFilePath, strRecvPath);
    }
    
    PDU* respdu = mkPDU();
    memcpy(respdu->caData, &ret, sizeof(bool));
    respdu->uiType = ENUM_MSG_TYPE_SHARE_FILE_AGREE_RESPOND;
    return respdu;
}

// 处理文件下载请求
PDU *MsgHandler::downloadFile()
{
    // 从PDU中获取文件名和路径
    QString strFileName = pdu->caData;
    QString strFilePath = pdu->caMsg;
    qDebug() << "下载文件路径:" << strFilePath;
    qDebug() << "下载文件名:" << strFileName;
    
    // 检查文件是否存在
    QFileInfo fileInfo(strFilePath);
    bool bSuccess = fileInfo.exists() && fileInfo.isFile();
    
    // 准备响应数据
    uint uiMsgLen = sizeof(bool) + sizeof(uint) + sizeof(uint); // success标志 + 文件大小 + 当前位置
    PDU* respdu = mkPDU(uiMsgLen);
    
    // 填充响应数据
    char* pData = (char*)respdu->caMsg;
    memcpy(pData, &bSuccess, sizeof(bool));
    pData += sizeof(bool);
    
    if (bSuccess) {
        // 文件存在，设置文件大小和初始位置
        uint uiFileSize = static_cast<uint>(fileInfo.size());
        uint uiCurPos = 0;
        
        memcpy(pData, &uiFileSize, sizeof(uint));
        pData += sizeof(uint);
        memcpy(pData, &uiCurPos, sizeof(uint));
        
        // 保存下载信息到成员变量
        m_strDownloadPath = strFilePath;
        m_iFileSize = fileInfo.size();
        m_iSentSize = 0;
        
        qDebug() << "文件存在，大小:" << fileInfo.size() << "字节";
    } else {
        // 文件不存在，设置默认值
        uint uiFileSize = 0;
        uint uiCurPos = 0;
        
        memcpy(pData, &uiFileSize, sizeof(uint));
        pData += sizeof(uint);
        memcpy(pData, &uiCurPos, sizeof(uint));
        
        qDebug() << "文件不存在:" << strFilePath;
    }
    
    // 复制文件名到caData
    memcpy(respdu->caData, strFileName.toStdString().c_str(), qMin(32, (int)strFileName.size()));
    respdu->uiType = ENUM_MSG_TYPE_DOWNLOAD_FILE_RESPOND;
    return respdu;
}

// 处理文件数据下载请求
PDU *MsgHandler::downloadFileData()
{
    // 打开文件
    QFile file(m_strDownloadPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "无法打开文件进行下载:" << m_strDownloadPath;
        return nullptr;
    }
    
    // 定位到上次发送的位置
    file.seek(m_iSentSize);
    
    // 读取数据（一次最多读取8192字节）
    QByteArray data = file.read(8192);
    int dataLen = data.size();
    bool bLastBlock = (dataLen == 0 || (m_iSentSize + dataLen) >= m_iFileSize);
    
    // 准备响应数据
    uint uiMsgLen = sizeof(bool) + sizeof(uint) + dataLen; // 是否最后一块 + 当前位置 + 数据
    PDU* respdu = mkPDU(uiMsgLen);
    
    // 填充响应数据
    char* pData = (char*)respdu->caMsg;
    memcpy(pData, &bLastBlock, sizeof(bool));
    pData += sizeof(bool);
    memcpy(pData, &m_iSentSize, sizeof(uint));
    pData += sizeof(uint);
    
    // 如果有数据，复制数据到PDU
    if (dataLen > 0) {
        memcpy(pData, data.data(), dataLen);
        
        // 更新已发送大小
        m_iSentSize += dataLen;
        
        qDebug() << "发送数据块，位置:" << m_iSentSize - dataLen << "长度:" << dataLen 
                 << "剩余:" << m_iFileSize - m_iSentSize;
    }
    
    respdu->uiType = ENUM_MSG_TYPE_DOWNLOAD_FILE_DATA;
    file.close();
    
    // 如果是最后一块，发送完成通知
    if (bLastBlock) {
        // 创建完成通知PDU
        PDU* completePdu = mkPDU(sizeof(bool));
        bool success = true;
        memcpy(completePdu->caMsg, &success, sizeof(bool));
        completePdu->uiType = ENUM_MSG_TYPE_DOWNLOAD_FILE_COMPLETE;
        
        // 立即发送完成通知
        m_tcpSocket->write((char*)completePdu, completePdu->uiPDULen);
        delete completePdu;
        
        qDebug() << "文件下载完成:" << m_strDownloadPath;
    }
    
    return respdu;
}

bool MsgHandler::copyDir(QString strSrcDir, QString strDestDir)
{
    QDir dir;
    
    // 创建目标目录
    if (!dir.mkdir(strDestDir)) {
        qDebug() << "无法创建目标目录：" << strDestDir;
        return false;
    }
    
    dir.setPath(strSrcDir);
    QFileInfoList fileInfoList = dir.entryInfoList();
    bool ret = true;
    
    for(int i=0; i<fileInfoList.size(); i++) {
        QFileInfo fileInfo = fileInfoList[i];
        QString fileName = fileInfo.fileName();
        
        // 跳过 . 和 ..
        if (fileName == "." || fileName == "..") {
            continue;
        }
        
        QString srcPath = strSrcDir + '/' + fileName;
        QString destPath = strDestDir + '/' + fileName;
        
        if (fileInfo.isFile()) {
            // 复制文件
            if (!QFile::copy(srcPath, destPath)) {
                qDebug() << "复制文件失败：" << srcPath << "到" << destPath;
                ret = false;
            } else {
                qDebug() << "成功复制文件：" << srcPath << "到" << destPath;
            }
        } else if (fileInfo.isDir()) {
            // 递归复制目录
            if (!copyDir(srcPath, destPath)) {
                qDebug() << "复制目录失败：" << srcPath << "到" << destPath;
                ret = false;
            }
        }
    }
    
    return ret;
}



// 处理分片上传请求
PDU* MsgHandler::uploadFileSlice()
{
    // 从PDU中解析参数
    // caData格式: [任务ID(64字节)][文件路径长度(4字节)][分片索引(4字节)][总分片数(4字节)][文件大小(8字节)]
    // caMsg格式: [文件路径(变长)][分片数据]
    char caTaskId[64] = {'\0'};
    int iPathLen = 0;
    int iSliceIndex = 0;
    int iTotalSlices = 0;
    qint64 iFileSize = 0;

    memcpy(caTaskId, pdu->caData, 64);
    memcpy(&iPathLen, pdu->caData + 64, sizeof(int));
    memcpy(&iSliceIndex, pdu->caData + 64 + sizeof(int), sizeof(int));
    memcpy(&iTotalSlices, pdu->caData + 64 + 2 * sizeof(int), sizeof(int));
    memcpy(&iFileSize, pdu->caData + 64 + 2 * sizeof(int) + sizeof(int), sizeof(qint64));

    QString strTaskId = caTaskId;
    
    // 从caMsg中提取文件路径（第一个分片包含路径信息）
    QString strUploadPath;
    if (iPathLen > 0 && iSliceIndex == 0) {
        char* pathData = new char[iPathLen + 1];
        memcpy(pathData, pdu->caMsg, iPathLen);
        pathData[iPathLen] = '\0';
        strUploadPath = QString(pathData);
        delete[] pathData;
        
        // 保存文件路径到任务映射
        m_uploadPaths[strTaskId] = strUploadPath;
        qDebug() << "保存上传路径：" << strUploadPath << "任务ID：" << strTaskId;
    } else {
        // 从任务映射中获取文件路径
        if (m_uploadPaths.contains(strTaskId)) {
            strUploadPath = m_uploadPaths[strTaskId];
        } else {
            qWarning() << "无法找到任务ID对应的上传路径：" << strTaskId;
            strUploadPath = m_strUploadPath; // 使用默认路径
        }
    }
    
    QString strTempFilePath = QString("%1.temp%2").arg(strUploadPath).arg(iSliceIndex);

    // 初始化任务信息（如果是第一个分片）
    if (!m_tempFiles.contains(strTaskId)) {
        // 使用空指针作为占位符，实际文件操作使用栈上的QFile对象
        m_tempFiles[strTaskId] = nullptr;
        m_totalFileSizes[strTaskId] = iFileSize;
        m_receivedSizes[strTaskId] = 0;
        m_totalSlices[strTaskId] = iTotalSlices;
        m_receivedSlices[strTaskId] = QSet<int>();
    }
    
    // 计算分片数据在caMsg中的偏移（第一个分片需要跳过路径）
    int dataOffset = (iSliceIndex == 0) ? iPathLen : 0;
    int actualDataLen = pdu->uiMsgLen - dataOffset;

    // 确保临时文件目录存在
    QDir dir;
    QString tempDir = QFileInfo(strTempFilePath).absolutePath();
    if (!dir.exists(tempDir)) {
        dir.mkpath(tempDir);
    }
    
    // 打开文件并写入数据（使用栈上的QFile对象，自动管理）
    QFile tempFile(strTempFilePath);
    bool ret = tempFile.open(QIODevice::WriteOnly);
    if (ret) {
        tempFile.write(pdu->caMsg + dataOffset, actualDataLen);
        tempFile.close();

        // 更新接收信息
        m_receivedSizes[strTaskId] += actualDataLen;
        m_receivedSlices[strTaskId].insert(iSliceIndex);

        qDebug() << "分片" << iSliceIndex << "上传成功，大小：" << actualDataLen;
    } else {
        qWarning() << "打开临时文件失败：" << strTempFilePath << "错误：" << tempFile.errorString();
    }

    // 创建响应PDU
    PDU* respdu = mkPDU(0);
    memcpy(respdu->caData, &ret, sizeof(ret));
    respdu->uiType = ENUM_MSG_TYPE_UPLOAD_FILE_SLICE_RESPOND;

    return respdu;
}

// 处理上传完成请求
PDU* MsgHandler::uploadFileComplete()
{
    char caTaskId[64] = {'\0'};
    char caFileName[32] = {'\0'};

    memcpy(caTaskId, pdu->caData, 64);
    memcpy(caFileName, pdu->caData + 64, 32);

    QString strTaskId = caTaskId;
    bool success = false; // 初始化success变量

    // 检查是否所有分片都已接收
    bool allSlicesReceived = true;
    if (m_receivedSlices.contains(strTaskId)) {
        for (int i = 0; i < m_totalSlices[strTaskId]; i++) {
            if (!m_receivedSlices[strTaskId].contains(i)) {
                allSlicesReceived = false;
                break;
            }
        }
    } else {
        allSlicesReceived = false;
    }

    // 从任务映射中获取文件路径
    QString strUploadPath;
    if (m_uploadPaths.contains(strTaskId)) {
        strUploadPath = m_uploadPaths[strTaskId];
    } else {
        qWarning() << "无法找到任务ID对应的上传路径：" << strTaskId << "，使用默认路径";
        strUploadPath = m_strUploadPath;
    }
    
    if (allSlicesReceived && m_receivedSizes.contains(strTaskId) &&
        m_receivedSizes[strTaskId] == m_totalFileSizes[strTaskId]) {

        // 确保目标文件目录存在
        QDir dir;
        QString targetDir = QFileInfo(strUploadPath).absolutePath();
        if (!dir.exists(targetDir)) {
            dir.mkpath(targetDir);
            qDebug() << "创建目标文件目录：" << targetDir;
        }

        // 合并所有分片
        QFile targetFile(strUploadPath);
        bool mergeSuccess = false;
        if (targetFile.open(QIODevice::WriteOnly)) {
            bool allSlicesRead = true;
            for (int i = 0; i < m_totalSlices[strTaskId]; i++) {
                QString slicePath = QString("%1.temp%2").arg(strUploadPath).arg(i);
                QFile sliceFile(slicePath);
                if (sliceFile.exists() && sliceFile.open(QIODevice::ReadOnly)) {
                    QByteArray sliceData = sliceFile.readAll();
                    if (targetFile.write(sliceData) != sliceData.size()) {
                        qWarning() << "写入分片" << i << "数据失败";
                        allSlicesRead = false;
                    }
                    sliceFile.close();
                    QFile::remove(slicePath); // 删除临时分片文件
                } else {
                    qWarning() << "无法打开分片文件：" << slicePath;
                    allSlicesRead = false;
                }
            }
            targetFile.close();
            mergeSuccess = allSlicesRead;
            
            if (mergeSuccess) {
                qDebug() << "文件" << caFileName << "分片上传完成并合并成功，路径：" << strUploadPath;
                
                // 计算并保存文件哈希，支持后续秒传
                QString md5 = calculateFileMD5(strUploadPath);
                if (!md5.isEmpty() && md5.size() == 32) {
                    OperateDB::getInstance().addFileHash(md5, strUploadPath, m_totalFileSizes[strTaskId], caFileName);
                    qDebug() << "文件哈希保存成功，MD5：" << md5;
                }
            } else {
                qWarning() << "文件合并失败，部分分片可能丢失";
            }
        } else {
            qWarning() << "无法打开目标文件：" << strUploadPath << "错误：" << targetFile.errorString();
        }

        // 清理任务数据
        if (m_tempFiles.contains(strTaskId)) {
            m_tempFiles.remove(strTaskId);
        }
        m_uploadPaths.remove(strTaskId);
        m_totalFileSizes.remove(strTaskId);
        m_receivedSizes.remove(strTaskId);
        m_totalSlices.remove(strTaskId);
        m_receivedSlices.remove(strTaskId);
        
        // 更新success标志
        success = mergeSuccess;
    } else {
        qWarning() << "分片接收不完整，无法合并。任务ID：" << strTaskId
                   << "已接收分片数：" << (m_receivedSlices.contains(strTaskId) ? m_receivedSlices[strTaskId].size() : 0)
                   << "总分片数：" << (m_totalSlices.contains(strTaskId) ? m_totalSlices[strTaskId] : 0);
    }

    // 创建响应PDU
    PDU* respdu = mkPDU(0);
    memcpy(respdu->caData, &success, sizeof(success));
    respdu->uiType = ENUM_MSG_TYPE_UPLOAD_FILE_COMPLETE_RESPOND;

    return respdu;
}
