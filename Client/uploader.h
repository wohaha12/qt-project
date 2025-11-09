#ifndef UPLOADER_H
#define UPLOADER_H

#include "protocol.h"

#include <QObject>
#include <QFile>
#include <QList>

class Uploader : public QObject
{
    Q_OBJECT
public:
    Uploader(QString strUploadFilePath, int threadCount = 3);
    void start();
    QString m_strUploadFilePath;
    QString m_strTaskId; // 任务ID（用于标识上传任务）
    QString m_strUploadPath; // 服务器端上传路径
    int m_iThreadCount; // 线程数量
    qint64 m_iFileSize; // 文件总大小
    qint64 m_iSliceSize; // 分片大小（默认为4MB）
    int m_iTotalSlices; // 总分片数
    QList<int> m_pendingSlices; // 待上传的分片索引
    QList<int> m_uploadedSlices; // 已上传的分片索引

public slots:
    void uploadFile();
    void uploadSlice(int sliceIndex);

private slots:
    void onSliceUploaded(int sliceIndex);
    void checkUploadComplete();

 signals:
    void finished();
    void errorMsgBox(QString strError);
    void uploadPDU(PDU* pdu);
    void uploadProgress(int progress);
    void sliceUploaded(int sliceIndex);
};

#endif // UPLOADER_H
