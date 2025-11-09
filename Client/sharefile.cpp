#include "client.h"
#include "index.h"
#include "sharefile.h"
#include "ui_sharefile.h"
#include <QDebug>

ShareFile::ShareFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShareFile)
{
    ui->setupUi(this);
}

void ShareFile::updataFriend_LW()
{
    ui->friend_LW->clear();
    QListWidget* friendList = Index::getInstance().getFriend()->getFriend_LW();
    for (int i=0;i<friendList->count();i++) {
        QListWidgetItem* pFriendItem = new QListWidgetItem(*friendList->item(i));
        ui->friend_LW->addItem(pFriendItem);
    }
}

ShareFile::~ShareFile()
{
    delete ui;
}


void ShareFile::on_allSelect_PB_clicked()
{
    for (int i = 0;i<ui->friend_LW->count();i++) {
        ui->friend_LW->item(i)->setSelected(true);
    }
}

void ShareFile::on_cancleSelec_PB_clicked()
{
    for (int i = 0;i<ui->friend_LW->count();i++) {
        ui->friend_LW->item(i)->setSelected(false);
    }
}

void ShareFile::on_confirm_PB_clicked()
{
    // 获取要分享的文件信息
    QString curFileName = Index::getInstance().getFile()->m_strShareFileName;
    QString curFilePath = Index::getInstance().getFile()->m_strCurPath;
    QString strPath = QString("%1/%2").arg(curFilePath).arg(curFileName);
    qDebug() << "分享文件路径:" << strPath;

    // 获取选中的好友列表
    QList<QListWidgetItem*> pItems = ui->friend_LW->selectedItems();
    if(pItems.isEmpty()) {
        QMessageBox::warning(this, "提示", "请选择要分享的好友");
        return;
    }

    QString strCurName = Client::getInstance().m_strLoginName;
    int friend_num = pItems.size();
    qDebug() << "选中好友数量:" << friend_num;

    // 计算PDU大小：好友数量*32 + 文件路径长度 + 1(结束符)
    int msgLen = friend_num * 32 + strPath.toStdString().size() + 1;
    PDU* pdu = mkPDU(msgLen);
    pdu->uiType = ENUM_MSG_TYPE_SHARE_FILE_REQUEST;

    // 在caData中存储当前用户名和好友数量
    memset(pdu->caData, 0, sizeof(pdu->caData));  // 清空caData
    memcpy(pdu->caData, strCurName.toStdString().c_str(), qMin(32, strCurName.length()));
    memcpy(pdu->caData + 32, &friend_num, sizeof(int));

    // 在caMsg中存储好友名称列表和文件路径
    char* pMsg = (char*)pdu->caMsg;
    memset(pMsg, 0, msgLen);  // 清空caMsg

    // 复制好友名称
    for(int i = 0; i < friend_num; i++) {
        QString friendName = pItems.at(i)->text();
        memcpy(pMsg + i * 32, friendName.toStdString().c_str(), 
               qMin(32, friendName.length()));
    }

    // 复制文件路径
    memcpy(pMsg + friend_num * 32, strPath.toStdString().c_str(), 
           strPath.length());

    // 发送请求
    Client::getInstance().sendMsg(pdu);

    // 关闭分享窗口
    this->hide();
}
