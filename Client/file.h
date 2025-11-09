#ifndef FILE_H
#define FILE_H

#include "protocol.h"
#include "sharefile.h"

#include <QWidget>
#include <qlistwidget.h>

namespace Ui {
class File;
}

class File : public QWidget
{
    Q_OBJECT

public:
    explicit File(QWidget *parent = nullptr);
    ~File();
    QString m_strUserPath;
    QString m_strCurPath;
    void flushFile();
    void updateFileList(QList<FileInfo*> pFileList);
    QList<FileInfo*> m_pFileList;
    QString m_strMvName;
    QString m_strMvPath;
    QString strSrcPath;
    QString strTarPath;
    QString m_strUploadFilePath;
    //QSet<QString> m_uploadedHashes;  // 本地去重哈希集合
    void uploadFile();
    ShareFile* m_pShareFile;
    QString m_strShareFileName;

    // 秒传相关方法 - 简化版本
    QString calculateFileMD5(const QString &filePath);


private slots:
    void on_mkDir_PB_clicked();
    void on_flushFile_PB_clicked();
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_return_PB_clicked();
    void on_mvFile_PB_clicked();
    void on_uploadFile_PB_clicked();
    void on_shareFile_PB_clicked();
    void on_deleteFile_PB_clicked();  // 新增删除文件按钮槽函数


public slots:
    void uploadError(QString strError);

    void updateUploadProgress(int progress);

private:
    Ui::File *ui;
};

#endif // FILE_H
