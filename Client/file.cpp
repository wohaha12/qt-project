#include "client.h"
#include "file.h"
#include "ui_file.h"
#include "uploader.h"

#include <QCryptographicHash>
#include <QFileDialog>
#include <qdir.h>
#include <qinputdialog.h>
#include <qmessagebox.h>
#include <QDateTime>

File::File(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::File)
{
    ui->setupUi(this);
    m_strUserPath = QString("%1/%2").arg(Client::getInstance().m_strRootPath).arg(Client::getInstance().m_strLoginName);
    m_strCurPath = m_strUserPath;
    flushFile();
    m_pShareFile = new ShareFile;
}

File::~File()
{
    delete ui;
    delete m_pShareFile;
}

void File::flushFile()
{
    PDU* pdu = mkPDU(m_strCurPath.toStdString().size()+1);
    pdu->uiType = ENUM_MSG_TYPE_FLUSH_FILE_REQUEST;
    memcpy(pdu->caMsg,m_strCurPath.toStdString().c_str(),m_strCurPath.toStdString().size());
    Client::getInstance().sendMsg(pdu);
}

void File::updateFileList(QList<FileInfo*> pFileList)
{
    foreach(FileInfo* pFileInfo,m_pFileList){
        delete pFileInfo;
    }
    m_pFileList = pFileList;
    ui->listWidget->clear();
    foreach(FileInfo* pFileInfo,pFileList){
        QListWidgetItem* pItem = new QListWidgetItem;
        if(pFileInfo->iType == 0){
            pItem->setIcon(QIcon(QPixmap(":/images/folder.png")));
        }else if(pFileInfo->iType == 1){
            pItem->setIcon(QIcon(QPixmap(":/images/file.png")));
        }
        pItem->setText(pFileInfo->caName);
        ui->listWidget->addItem(pItem);
    }
}

void File::on_mkDir_PB_clicked()
{
    QString strNewDir = QInputDialog::getText(this,"提示","新建文件夹名");
    if(strNewDir.toStdString().size() == 0 || strNewDir.toStdString().size()>32){
        QMessageBox::information(this,"提示","文件夹名长度非法");
        return;
    }
    PDU* pdu = mkPDU(m_strCurPath.toStdString().size()+1);
    pdu->uiType = ENUM_MSG_TYPE_MKDIR_REQUEST;
    memcpy(pdu->caData,strNewDir.toStdString().c_str(),32);
    memcpy(pdu->caMsg,m_strCurPath.toStdString().c_str(),m_strCurPath.toStdString().size());
    Client::getInstance().sendMsg(pdu);
}

void File::on_flushFile_PB_clicked()
{
    flushFile();
}

void File::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    foreach(FileInfo* pFileInfo,m_pFileList){
        if(item->text() == pFileInfo->caName && pFileInfo->iType != 0){
            return;
        }
    }
    m_strCurPath = m_strCurPath + "/" + item->text();
    flushFile();
}

void File::on_return_PB_clicked()
{
    // 检查当前路径是否已经是用户根目录
//    if (m_strCurPath != m_strUserPath) {
//        // 找到最后一个 '/' 的位置
//        int lastIndex = m_strCurPath.lastIndexOf('/');
//        if (lastIndex != -1) {
//            // 截取到最后一个 '/' 之前的路径，即父目录
//            flushFile();
//        }
//    } else {
//        QMessageBox::information(this, "提示", "已经是根目录");
//    }
    if (m_strCurPath != m_strUserPath) {
        int lastIndex = m_strCurPath.lastIndexOf('/');
        m_strCurPath.remove(lastIndex,m_strCurPath.size() - lastIndex);
        flushFile();
    } else {
        QMessageBox::information(this, "提示", "已经是根目录");
    }
}

void File::on_mvFile_PB_clicked()
{
    //判断按钮上的文字，分为移动文件和确认/取消
    if(ui->mvFile_PB->text() == "移动文件"){
        QListWidgetItem* pItem = ui->listWidget->currentItem();
        if(pItem == NULL){
            return;
        }
        QMessageBox::information(this,"移动文件","请选择要移动到的目录");
        //修改按钮上的文字
        ui->mvFile_PB->setText("确认/取消");
        //记录选择的文件名以及该文件的当前路径
        m_strMvName = pItem->text();
        m_strMvPath = m_strCurPath;
        return;
    }
    //点击确认/取消时的逻辑，修改按钮的文字
    ui->mvFile_PB->setText("移动文件");
    //如果用户选择了目录就移动到该目录下，如果没有选择目录（没有选择或者选择的是文件）就移动到当前路径
    QListWidgetItem* pItem = ui->listWidget->currentItem();
    //获取要移动到的目录
    QString strTarPath;
    if(pItem == NULL){
        strTarPath = m_strCurPath;
    }else{
        strTarPath = m_strCurPath + "/" + pItem->text();
        foreach(FileInfo* pFileInfo,m_pFileList){
            if(pItem->text() == pFileInfo->caName && pFileInfo->iType != 0){
                strTarPath = m_strCurPath;
                break;
            }
        }
    }
    //询问用户是否确认移动
    int ret = QMessageBox::information(this,"移动文件",QString("是否确认移动到\n%1?").arg(strTarPath),"确认","取消");
    if(ret != 0){
        return;
    }
    //拼接文件的原完整路径和要移动到的完整路径
    strTarPath = strTarPath + "/" + m_strMvName;
    QString strSrcPath = m_strMvPath + "/" + m_strMvName;
    //两个路径放入caMsg，将两个路径的长度放入caData中，服务器根据路径的长度获取路径
    int iSrcPathLen = strSrcPath.toStdString().size();
    int iTarPathLen = strTarPath.toStdString().size();
    qDebug()<<"iSrcPathLen"<<iSrcPathLen<<"iTarPathLen"<<iTarPathLen;
    PDU* pdu = mkPDU(iSrcPathLen+iTarPathLen+1);
    pdu->uiType = ENUM_MSG_TYPE_MOVE_FILE_REQUEST;
    memcpy(pdu->caData,&iSrcPathLen,sizeof(int));
    memcpy(pdu->caData+sizeof(int),&iTarPathLen,sizeof(int));
    memcpy(pdu->caMsg,strSrcPath.toStdString().c_str(),iSrcPathLen);
    memcpy(pdu->caMsg+iSrcPathLen,strTarPath.toStdString().c_str(),iTarPathLen);
    Client::getInstance().sendMsg(pdu);
}

//void File::on_uploadFile_PB_clicked()
//{
//    m_strUploadFilePath = QFileDialog::getOpenFileName(this,"选择文件","","所有文件(*.*)");
//    qDebug()<<"selectedPath"<<m_strUploadFilePath;
//    int index = m_strUploadFilePath.lastIndexOf("/");
//    QString strFileName = m_strUploadFilePath.right(m_strUploadFilePath.size() - index -1);
//    qDebug()<<"selectFileName"<<strFileName;
//    QFile file(m_strUploadFilePath);
//    qint64 iFileSize = file.size();
//    PDU* pdu = mkPDU(m_strCurPath.toStdString().size());
//    pdu->uiType = ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST;
//    memcpy(pdu->caMsg, m_strCurPath.toStdString().c_str(), m_strCurPath.toStdString().size());
//    memcpy(pdu->caData,strFileName.toStdString().c_str(),32);
//    memcpy(pdu->caData + 32,&iFileSize,sizeof(qint64));
//    Client::getInstance().sendMsg(pdu);
//}


// 计算文件MD5的方法
QString File::calculateFileMD5(const QString &filePath)
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

// 修改文件上传按钮点击事件，增加MD5计算
void File::on_uploadFile_PB_clicked()
{
    // 通过文件选择弹窗获取要上传文件的路径
    m_strUploadFilePath = QFileDialog::getOpenFileName(this, "选择文件", "", "所有文件(*.*)");
    if (m_strUploadFilePath.isEmpty()) {
        qDebug() << "未选择文件";
        return;
    }

    QFileInfo fileInfo(m_strUploadFilePath);
    QString strFileName = fileInfo.fileName();
    qint64 iFileSize = fileInfo.size();

    // 计算文件MD5
    QString md5 = calculateFileMD5(m_strUploadFilePath);
    qDebug() << "文件MD5: " << md5;

    // 先查询MD5是否已存在，支持秒传
    if (!md5.isEmpty()) {
        PDU* queryPdu = mkPDU(md5.size() + 1);
        queryPdu->uiType = ENUM_MSG_TYPE_QUERY_FILE_MD5_REQUEST;
        memcpy(queryPdu->caMsg, md5.toStdString().c_str(), md5.size() + 1);
        Client::getInstance().sendMsg(queryPdu);
        return; // 等待MD5查询响应后再决定上传方式
    }

    // 如果MD5计算失败，直接进行分片上传
    uploadFile();
}




//void File::uploadFile()
//{
//   Uploader* uploader = new Uploader(m_strUploadFilePath);
//   connect(uploader,&Uploader::errorMsgBox,this,&File::uploadError);
//   connect(uploader,&Uploader::uploadPDU,&Client::getInstance(),&Client::sendMsg);
//   uploader->start();
//}

// 在File类的uploadFile方法中
void File::uploadFile()
{
    if (m_strUploadFilePath.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择文件");
        return;
    }

    QFileInfo fileInfo(m_strUploadFilePath);
    QString strFileName = fileInfo.fileName();
    qint64 iFileSize = fileInfo.size();
    QString strServerPath = QString("%1/%2").arg(m_strCurPath).arg(strFileName);

    // 计算文件MD5
    QString md5 = calculateFileMD5(m_strUploadFilePath);
    
    // 生成任务ID
    QString strTaskId = QUuid::createUuid().toString().replace("{", "").replace("}", "").replace("-", "");
    
    // 先创建上传请求，告诉服务器要上传文件
    uint msgLen = m_strCurPath.toStdString().size() + md5.size() + 1;
    PDU* pdu = mkPDU(msgLen);
    pdu->uiType = ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST;
    memset(pdu->caData, 0, 32);
    memcpy(pdu->caData, strFileName.toStdString().c_str(), qMin(32, (int)strFileName.size()));
    memcpy(pdu->caData + 32, &iFileSize, sizeof(qint64));
    
    // 存储当前路径和MD5到caMsg
    memset(pdu->caMsg, 0, msgLen);
    memcpy(pdu->caMsg, m_strCurPath.toStdString().c_str(), m_strCurPath.toStdString().size());
    memcpy(pdu->caMsg + m_strCurPath.toStdString().size(), md5.toStdString().c_str(), md5.size());
    pdu->caMsg[msgLen - 1] = '\0';
    
    // 保存任务ID和MD5信息，用于后续上传分片
    m_strTaskId = strTaskId;
    m_strFileMD5 = md5;
    
    // 发送上传请求
    Client::getInstance().sendMsg(pdu);
    
    // 创建上传器实例
    Uploader* uploader = new Uploader(m_strUploadFilePath, 4); // 使用4个线程
    uploader->m_strTaskId = strTaskId;
    uploader->m_strUploadPath = strServerPath;
    
    // 连接信号槽
    connect(uploader, &Uploader::errorMsgBox, this, &File::uploadError);
    connect(uploader, &Uploader::uploadPDU, &Client::getInstance(), &Client::sendMsg);
    connect(uploader, &Uploader::uploadProgress, this, &File::updateUploadProgress);
    
    // 当上传完成时，刷新文件列表
    connect(uploader, &Uploader::finished, this, [this]() {
        flushFile();  // 刷新文件列表
        QMessageBox::information(this, "提示", "文件上传完成");
    });
    
    // 确保资源正确释放
    connect(uploader, &Uploader::finished, uploader, &Uploader::deleteLater);
    
    // 开始上传
    uploader->start();
    
    qDebug() << "开始上传文件：" << strFileName;
    qDebug() << "任务ID：" << strTaskId;
    qDebug() << "目标路径：" << strServerPath;
    qDebug() << "文件MD5：" << md5;
}

// 添加一个新的槽函数来更新上传进度
void File::updateUploadProgress(int progress)
{
    // 这里可以添加进度条更新的代码
    qDebug() << "上传进度: " << progress << "%";
}




void File::uploadError(QString strError)
{
    QMessageBox::warning(this,"提示",strError);
}


/*
void File::on_uploadFile_PB_clicked()
{
    //通过文件选择弹窗获取要上传文件的路径，路径作为file的属性存下来
    // 打开选择文件的对话框
    m_strUploadFilePath = QFileDialog::getOpenFileName(this,"选择文件","","所有文件(*.*)");
    if (m_strUploadFilePath.isEmpty()) {
        qDebug() << "未选择文件";
        return;
    }
    qDebug()<<"selectedPath"<<m_strUploadFilePath;
//    int lastIndexOf = selectedPath.lastIndexOf("/");
//    QString selectFileName = selectedPath.right(selectedPath.size() - lastIndexOf -1);
//    qDebug()<<"selectFileName"<<selectFileName;
    QFileInfo fileInfo(m_strUploadFilePath);
    QString selectFileName = fileInfo.fileName();
    qDebug() << "selectFileName:" << selectFileName;
    qint64 fileByte = fileInfo.size();
    qDebug() << "fileByte:" << fileByte;
    //计算文件MD5哈希值（用于去重和校验）
    QFile file(m_strUploadFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "打开文件失败：" << file.errorString();
        return;
    }
    QCryptographicHash hash(QCryptographicHash::Md5);
    const int bufferSize = 4096;
    char buffer[bufferSize];
    while (!file.atEnd()) {
        qint64 bytesRead = file.read(buffer, bufferSize);
        if (bytesRead > 0) {
            hash.addData(buffer, bytesRead);
        }
    }
    file.close();
    QByteArray hashBytes = hash.result();
    QString fileHash = hashBytes.toHex();  // 32字节十六进制哈希字符串
    qDebug() << "文件哈希值：" << fileHash;
    //上传前检查是否重复（本地哈希集合判断）
    if (m_uploadedHashes.contains(fileHash)) {
        qDebug() << "文件已上传，无需重复上传";
        QMessageBox::information(this,"提示","该文件已上传，无需重复上传");
        return;
    }
    // 用 QByteArray 存储含 \0 的数据
    QByteArray caMsgData;
    caMsgData.append(m_strCurPath);
    caMsgData.append('\0');  // 路径终止符
    caMsgData.append(fileHash);  // 哈希值
//    int nullIndex = caMsgData.indexOf('\0');
//    if (nullIndex == -1) {
//        qDebug() << "未找到分隔符\x00，无法提取哈希";
//        return;
//    }
//    QByteArray hashPart = caMsgData.mid(nullIndex + 1);
//    if (hashPart.size() != 32) {
//        qDebug() << "哈希部分长度异常：" << hashPart.size() << "字节（预期32字节）";
//    }
//    qDebug() << "单独的哈希值：" << QString(hashPart);
//    qDebug() << "哈希值的十六进制：" << hashPart.toHex();  // 这里会显示哈希字符串每个字符的十六进制编码

    // 创建 PDU 时使用 QByteArray 的实际长度（含所有 \0 和哈希）
    PDU* pdu = mkPDU(caMsgData.size());
    pdu->uiType = ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST;
    // 复制完整数据（memcpy 会按指定长度复制，无视 \0）
    memcpy(pdu->caMsg, caMsgData.constData(), caMsgData.size());
    memcpy(pdu->caData,selectFileName.toStdString().c_str(),32);
    // 用QByteArray存储文件大小（8字节），剩余24字节补0
    QByteArray fileSizeData;
    QDataStream sizeStream(&fileSizeData, QIODevice::WriteOnly);
    sizeStream << fileByte;  // 写入8字节的fileByte（27770）
    fileSizeData.resize(32);  // 不足32字节自动补0
    memcpy(pdu->caData + 32, fileSizeData.constData(), 32);
    Client::getInstance().sendMsg(pdu);
    m_uploadedHashes.insert(fileHash);
}
*/

void File::on_shareFile_PB_clicked()
{
    QListWidgetItem* pItem = ui->listWidget->currentItem();
    if(pItem == NULL){
        return;
    }
    m_strShareFileName = pItem->text();
    m_pShareFile->updataFriend_LW();
    if(m_pShareFile->isHidden()){
        m_pShareFile->show();
    }
}

void File::on_deleteFile_PB_clicked()
{
    QListWidgetItem* pItem = ui->listWidget->currentItem();
    if(pItem == NULL){
        QMessageBox::warning(this, "提示", "请先选择要删除的文件或文件夹");
        return;
    }

    // 确认是否删除
    int ret = QMessageBox::question(this, "删除确认", 
                                  QString("确定要删除 %1 吗？").arg(pItem->text()),
                                  QMessageBox::Yes | QMessageBox::No);
    if(ret != QMessageBox::Yes){
        return;
    }

    // 获取完整文件路径
    QString strFullPath = m_strCurPath + "/" + pItem->text();
    
    // 创建并发送删除文件请求
    PDU* pdu = mkPDU(strFullPath.toStdString().size() + 1);
    pdu->uiType = ENUM_MSG_TYPE_DELETE_FILE_REQUEST;
    memcpy(pdu->caMsg, strFullPath.toStdString().c_str(), strFullPath.toStdString().size());
    
    // 在文件信息中查找是否为文件夹
    bool isDir = false;
    foreach(FileInfo* pFileInfo, m_pFileList) {
        if(pItem->text() == pFileInfo->caName) {
            isDir = (pFileInfo->iType == 0);  // 0表示文件夹
            break;
        }
    }
    
    // 在caData中标记是文件还是文件夹
    memcpy(pdu->caData, &isDir, sizeof(bool));
    
    Client::getInstance().sendMsg(pdu);
    
    // 删除后刷新文件列表
    flushFile();
}



// 下载文件按钮槽函数实现
void File::on_downloadFile_PB_clicked()
{
    QListWidgetItem* pItem = ui->listWidget->currentItem();
    if(pItem == NULL){
        QMessageBox::warning(this, "提示", "请先选择要下载的文件");
        return;
    }

    // 检查是否是文件夹
    bool isDir = false;
    foreach(FileInfo* pFileInfo, m_pFileList) {
        if(pItem->text() == pFileInfo->caName) {
            isDir = (pFileInfo->iType == 0);  // 0表示文件夹
            break;
        }
    }

    if(isDir){
        QMessageBox::warning(this, "提示", "暂不支持文件夹下载");
        return;
    }

    // 获取完整文件路径
    QString strFileName = pItem->text();
    QString strFullPath = m_strCurPath + "/" + strFileName;

    // 选择保存位置
    QString strSavePath = QFileDialog::getSaveFileName(this, "保存文件", "./" + strFileName, "所有文件(*.*)");
    if(strSavePath.isEmpty()){
        qDebug() << "未选择保存位置";
        return;
    }

    qDebug() << "准备下载文件：" << strFileName;
    qDebug() << "服务器路径：" << strFullPath;
    qDebug() << "本地保存路径：" << strSavePath;

    // 发送下载文件请求
    uint msgLen = strFullPath.toStdString().size() + 1;
    PDU* pdu = mkPDU(msgLen);
    pdu->uiType = ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST;
    memcpy(pdu->caData, strFileName.toStdString().c_str(), qMin(32, (int)strFileName.size()));
    memcpy(pdu->caMsg, strFullPath.toStdString().c_str(), strFullPath.toStdString().size() + 1);
    
    // 保存下载信息
    m_strDownloadFileName = strFileName;
    m_strDownloadSavePath = strSavePath;
    
    Client::getInstance().sendMsg(pdu);
    
    // 这里可以添加下载进度提示
    QMessageBox::information(this, "提示", "开始下载文件");
}

// 重命名按钮槽函数实现
void File::on_rename_PB_clicked()
{
    QListWidgetItem* pItem = ui->listWidget->currentItem();
    if(pItem == NULL){
        QMessageBox::warning(this, "提示", "请先选择要重命名的文件或文件夹");
        return;
    }

    QString strOldName = pItem->text();
    QString strOldPath = m_strCurPath + "/" + strOldName;
    
    // 检查选中的是文件还是文件夹
    bool isDir = false;
    foreach(FileInfo* pFileInfo, m_pFileList) {
        if(strOldName == pFileInfo->caName) {
            isDir = (pFileInfo->iType == 0);  // 0表示文件夹
            break;
        }
    }

    // 获取文件扩展名（如果是文件）
    QString strExt = "";
    if(!isDir) {
        int dotIndex = strOldName.lastIndexOf('.');
        if(dotIndex != -1) {
            strExt = strOldName.right(strOldName.size() - dotIndex);
            strOldName = strOldName.left(dotIndex);
        }
    }

    // 弹出重命名对话框
    bool ok;
    QString strNewName = QInputDialog::getText(this, "重命名", 
                                             "请输入新名称：", 
                                             QLineEdit::Normal, 
                                             strOldName, 
                                             &ok);
    
    if(ok && !strNewName.isEmpty()) {
        // 如果是文件，添加回扩展名
        if(!isDir && !strExt.isEmpty()) {
            strNewName += strExt;
        }

        // 验证文件名是否有效
        if(strNewName.contains('/') || strNewName.contains('\\') || 
           strNewName.contains(':') || strNewName.contains('*') || 
           strNewName.contains('?') || strNewName.contains('<') || 
           strNewName.contains('>') || strNewName.contains('|') || 
           strNewName.contains('"')) {
            QMessageBox::warning(this, "错误", "文件名包含非法字符");
            return;
        }

        // 检查新名称是否与当前目录下的其他文件或文件夹重名
        foreach(FileInfo* pFileInfo, m_pFileList) {
            if(strNewName == pFileInfo->caName && strOldName + strExt != pFileInfo->caName) {
                QMessageBox::warning(this, "错误", "新名称与现有文件或文件夹重名");
                return;
            }
        }

        // 构建新路径
        QString strNewPath = m_strCurPath + "/" + strNewName;
        
        qDebug() << "重命名：" << strOldPath << " -> " << strNewPath;
        
        // 发送重命名请求
        uint uiOldNameLen = strOldName.toStdString().size() + 1;
        uint uiNewNameLen = strNewName.toStdString().size() + 1;
        uint uiPathLen = m_strCurPath.toStdString().size() + 1;
        uint uiMsgLen = uiOldNameLen + uiNewNameLen + uiPathLen + sizeof(bool);
        
        PDU* pdu = mkPDU(uiMsgLen);
        pdu->uiType = ENUM_MSG_TYPE_RENAME_FILE_REQUEST;
        
        // 填充消息数据
        char* pData = pdu->caMsg;
        memcpy(pData, &isDir, sizeof(bool));  // 先传输是否为文件夹
        pData += sizeof(bool);
        
        memcpy(pData, strOldName.toStdString().c_str(), uiOldNameLen);
        pData += uiOldNameLen;
        
        memcpy(pData, strNewName.toStdString().c_str(), uiNewNameLen);
        pData += uiNewNameLen;
        
        memcpy(pData, m_strCurPath.toStdString().c_str(), uiPathLen);
        
        Client::getInstance().sendMsg(pdu);
        
        // 这里可以添加等待响应的处理
    }
}
