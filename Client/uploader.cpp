//#include "uploader.h"

//#include <QFile>
//#include <QThread>

//Uploader::Uploader(QString strUploadFilePath)
//{
//    m_strUploadFilePath = strUploadFilePath;
//}

//void Uploader::start()
//{
//    QThread* thread = new QThread;
//    this->moveToThread(thread);
//    connect(thread,&QThread::started,this,&Uploader::uploadFile);
//    connect(this,&Uploader::finished,thread,&QThread::quit);
//    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
//    thread->start();
//}

//void Uploader::uploadFile()
//{
//    QFile file(m_strUploadFilePath);
//    if(!file.open(QIODevice::ReadOnly)){
//        emit errorMsgBox("上传文件:打开文件失败");
//        emit finished();
//        return;
//    }
//    while (true) {
//        PDU* pdu = mkPDU(4096);
//        pdu->uiType = ENUM_MSG_TYPE_UPLOAD_FILE_DATA_REQUEST;
//        qint64 ret = file.read(pdu->caMsg,4094);
//        if(ret == 0){
//            break;
//        }
//        if(ret < 0){
//            emit errorMsgBox("上传文件:读取文件失败");
//            break;
//        }
//        pdu->uiMsgLen = ret;
//        pdu->uiPDULen = ret + sizeof(PDU);
//        emit uploadPDU(pdu);
//    }
//    file.close();
//    emit finished();
//}


#include "uploader.h"
#include "memorypool.h"
#include <QFile>
#include <QThread>
#include <QDebug>
#include <QFileInfo>
#include <QDateTime>

Uploader::Uploader(QString strUploadFilePath, int threadCount)
    : m_strUploadFilePath(strUploadFilePath), m_iThreadCount(threadCount)
{
    m_iSliceSize = 4 * 1024 * 1024; // 4MB per slice
    QFile file(m_strUploadFilePath);
    if(file.exists()) {
        m_iFileSize = file.size();
        m_iTotalSlices = (m_iFileSize + m_iSliceSize - 1) / m_iSliceSize;
    }
    connect(this, &Uploader::sliceUploaded, this, &Uploader::onSliceUploaded);
}

void Uploader::start()
{
    // 初始化待上传分片列表
    for(int i = 0; i < m_iTotalSlices; i++) {
        m_pendingSlices.append(i);
    }
    m_uploadedSlices.clear();

    // 创建并启动多个线程
    for(int i = 0; i < m_iThreadCount && !m_pendingSlices.isEmpty(); i++) {
        int sliceIndex = m_pendingSlices.takeFirst();
        QThread* thread = new QThread;
        Uploader* sliceUploader = new Uploader(m_strUploadFilePath, 1);
        sliceUploader->moveToThread(thread);

        connect(thread, &QThread::started, [=]() {
            sliceUploader->uploadSlice(sliceIndex);
        });
        connect(sliceUploader, &Uploader::uploadPDU, this, &Uploader::uploadPDU);
        connect(sliceUploader, &Uploader::errorMsgBox, this, &Uploader::errorMsgBox);
        connect(sliceUploader, &Uploader::finished, thread, &QThread::quit);
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);
        connect(thread, &QThread::finished, sliceUploader, &Uploader::deleteLater);
        connect(sliceUploader, &Uploader::sliceUploaded, this, &Uploader::sliceUploaded);

        thread->start();
    }
}

void Uploader::uploadFile()
{
    // 单线程上传的原始方法保持不变，但主要逻辑已移至uploadSlice
    if(m_iTotalSlices <= 1) {
        uploadSlice(0);
    }
}

void Uploader::uploadSlice(int sliceIndex)
{
    QFile file(m_strUploadFilePath);
    if(!file.open(QIODevice::ReadOnly)) {
        emit errorMsgBox(QString("上传分片%1:打开文件失败").arg(sliceIndex));
        emit finished();
        return;
    }

    // 计算分片的偏移量和大小
    qint64 offset = sliceIndex * m_iSliceSize;
    qint64 sliceSize = qMin(m_iSliceSize, m_iFileSize - offset);

    // 设置文件指针到分片起始位置
    if(!file.seek(offset)) {
        emit errorMsgBox(QString("上传分片%1:文件定位失败").arg(sliceIndex));
        file.close();
        emit finished();
        return;
    }

    // 计算需要的数据大小：路径长度 + 分片数据
    int pathLen = (sliceIndex == 0) ? m_strUploadPath.toStdString().size() : 0;
    int totalDataSize = pathLen + sliceSize;
    
    // 上传分片数据
    // caData格式: [任务ID(64字节)][路径长度(4字节)][分片索引(4字节)][总分片数(4字节)][文件大小(8字节)]
    // caMsg格式: [文件路径(仅第一个分片)][分片数据]
    PDU* pdu = mkPDU(totalDataSize);
    pdu->uiType = ENUM_MSG_TYPE_UPLOAD_FILE_SLICE_REQUEST;

    // 在caData中存储任务ID、路径长度、分片索引、总分片数和文件大小
    memset(pdu->caData, 0, 64); // 清空任务ID区域
    memcpy(pdu->caData, m_strTaskId.toStdString().c_str(), qMin(63, m_strTaskId.size()));
    memcpy(pdu->caData + 64, &pathLen, sizeof(int));
    memcpy(pdu->caData + 64 + sizeof(int), &sliceIndex, sizeof(int));
    memcpy(pdu->caData + 64 + 2 * sizeof(int), &m_iTotalSlices, sizeof(int));
    memcpy(pdu->caData + 64 + 2 * sizeof(int) + sizeof(int), &m_iFileSize, sizeof(qint64));

    // 在caMsg中存储文件路径（仅第一个分片）和分片数据
    int msgOffset = 0;
    if (sliceIndex == 0 && pathLen > 0) {
        memcpy(pdu->caMsg, m_strUploadPath.toStdString().c_str(), pathLen);
        msgOffset = pathLen;
    }
    
    // 读取分片数据到caMsg的偏移位置
    qint64 ret = file.read(pdu->caMsg + msgOffset, sliceSize);
    file.close();

    if(ret < 0) {
        emit errorMsgBox(QString("上传分片%1:读取文件失败").arg(sliceIndex));
        MemoryPool::getInstance().deallocatePDU(pdu);
        emit finished();
        return;
    }

    pdu->uiMsgLen = pathLen + ret;
    pdu->uiPDULen = sizeof(PDU) + pdu->uiMsgLen;

    emit uploadPDU(pdu);
    emit sliceUploaded(sliceIndex);
    emit finished();
}

void Uploader::onSliceUploaded(int sliceIndex)
{
    m_uploadedSlices.append(sliceIndex);

    // 检查是否有更多分片需要上传
    if(!m_pendingSlices.isEmpty()) {
        int nextSlice = m_pendingSlices.takeFirst();
        uploadSlice(nextSlice);
    } else {
        // 检查是否所有分片都已上传完成
        checkUploadComplete();
    }

    // 发送上传进度
    int progress = (m_uploadedSlices.size() * 100) / m_iTotalSlices;
    emit uploadProgress(progress);
}

void Uploader::checkUploadComplete()
{
    if(m_uploadedSlices.size() == m_iTotalSlices) {
        // 所有分片上传完成，通知服务器合并文件
        // caData格式: [任务ID(64字节)][文件名(32字节)]
        QFileInfo fileInfo(m_strUploadFilePath);
        QString fileName = fileInfo.fileName();
        
        PDU* pdu = mkPDU(0);
        pdu->uiType = ENUM_MSG_TYPE_UPLOAD_FILE_COMPLETE_REQUEST;
        memset(pdu->caData, 0, 64);
        memcpy(pdu->caData, m_strTaskId.toStdString().c_str(), qMin(63, m_strTaskId.size()));
        memset(pdu->caData + 64, 0, 32);
        memcpy(pdu->caData + 64, fileName.toStdString().c_str(), qMin(31, fileName.size()));
        emit uploadPDU(pdu);
        emit finished();
    }
}
