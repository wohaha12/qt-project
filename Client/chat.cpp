#include "chat.h"
#include "client.h"
#include "protocol.h"
#include "ui_chat.h"
#include <QMessageBox>

Chat::Chat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chat)
{
    ui->setupUi(this);
}

Chat::~Chat()
{
    delete ui;
}

void Chat::on_Send_PB_clicked()
{
    QString strMsg = ui->Msg->text();
    if (strMsg.isEmpty()) {
        return;
    }
    ui->Msg->clear();
    
    // 先显示自己发送的消息
    QString displayMsg = QString("%1 : %2").arg(Client::getInstance().m_strLoginName).arg(strMsg);
    ui->Show->append(displayMsg);
    
    // 发送消息到服务器
    PDU* pdu = mkPDU(strMsg.toStdString().size() + 1);
    if (pdu == NULL) {
        QMessageBox::critical(this, "错误", "内存分配失败");
        return;
    }
    pdu->uiType = ENUM_MSG_TYPE_CHAT_REQUEST;
    memcpy(pdu->caData, Client::getInstance().m_strLoginName.toStdString().c_str(), 32);
    memcpy(pdu->caData+32, m_strChatName.toStdString().c_str(), 32);
    memcpy(pdu->caMsg, strMsg.toStdString().c_str(), strMsg.toStdString().size());
    pdu->caMsg[strMsg.toStdString().size()] = '\0';
    Client::getInstance().sendMsg(pdu);
}


void Chat::addMsg(QString strMsg)
{
    ui->Show->append(strMsg);
}
