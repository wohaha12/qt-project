#include "clienttask.h"
#include "memorypool.h"
#include "mytcpserver.h"
#include "mytcpsocket.h"
#include <QDebug>

MyTcpServer &MyTcpServer::getInstance()
{
    static MyTcpServer instance;
    return instance;
}

//void MyTcpServer::incomingConnection(qintptr handle)
//{
//    qDebug()<<"新客户端连接";
//    MyTcpSocket* psocket = new MyTcpSocket;
//    psocket->setSocketDescriptor(handle);
//    m_tcpSocketList.append(psocket);

////    for(int i = 0;i<m_tcpSocketList.size();i++){
////        qDebug()<<m_tcpSocketList[i];
////    }
//    ClientTask* task = new ClientTask(psocket);
//    threadPool->start(task);

//}



void MyTcpServer::incomingConnection(qintptr handle)
{
    qDebug()<<"新客户端连接";
    MyTcpSocket* psocket = new MyTcpSocket;
    psocket->setSocketDescriptor(handle);
    m_tcpSocketList.append(psocket);

    // 直接在主线程中连接信号和槽，不要移动对象到线程池
    connect(psocket, &QTcpSocket::readyRead, psocket, &MyTcpSocket::recvMsg);
    connect(psocket, &QTcpSocket::disconnected, psocket, &MyTcpSocket::clientOffline);

    // 移除ClientTask相关代码，不需要将socket移动到线程池
    // ClientTask* task = new ClientTask(psocket);
    // threadPool->start(task);
}




void MyTcpServer::deleteSocket(MyTcpSocket *mysocket)
{
    m_tcpSocketList.removeOne(mysocket);
    mysocket->deleteLater();
    mysocket = NULL;
}

//void MyTcpServer::resend(char* strName, PDU *pdu)
//{
//    if(strName == NULL || pdu == NULL){
//        return;
//    }
//    for(int i = 0;i<m_tcpSocketList.size();i++){
//        if(QString(strName) == m_tcpSocketList[i]->m_strLoginName){
//            m_tcpSocketList[i]->write((char*)pdu,pdu->uiPDULen);
//            qDebug() << "resendMsg pdu->uiPDULen" <<pdu->uiPDULen
//                     << "pdu->uiMsgLen" <<pdu->uiMsgLen
//                     << "pdu->uiType" <<pdu->uiType
//                     << "pdu->caData" <<pdu->caData
//                     << "pdu->caData + 32" <<pdu->caData + 32
//                     << "pdu->caMsg" <<pdu->caMsg;
//            break;
//        }
//    }
//}

void MyTcpServer::resend(char* strName, PDU *pdu, bool autoRelease) // 添加autoRelease参数，默认为false
{
    if(strName == NULL || pdu == NULL){
        return;
    }

    // 创建一个PDU副本，使用内存池分配内存
    PDU* pduCopy = MemoryPool::getInstance().allocatePDU(pdu->uiMsgLen);
    if (pduCopy) {
        memcpy(pduCopy, pdu, pdu->uiPDULen);

        for(int i = 0; i < m_tcpSocketList.size(); i++){
            if(QString(strName) == m_tcpSocketList[i]->m_strLoginName){
                m_tcpSocketList[i]->write((char*)pduCopy, pduCopy->uiPDULen);
                qDebug() << "resendMsg pdu->uiPDULen" << pduCopy->uiPDULen
                         << "pdu->uiMsgLen" << pduCopy->uiMsgLen
                         << "pdu->uiType" << pduCopy->uiType
                         << "pdu->caData" << pduCopy->caData
                         << "pdu->caData + 32" << pduCopy->caData + 32
                         << "pdu->caMsg" << pduCopy->caMsg;
                break;
            }
        }

        // 释放副本PDU对象
        MemoryPool::getInstance().deallocatePDU(pduCopy);
    }

    // 如果需要自动释放原始PDU对象
    if (autoRelease) {
        MemoryPool::getInstance().deallocatePDU(pdu);
    }
}


MyTcpServer::MyTcpServer()
{
    threadPool = QThreadPool::globalInstance();
    threadPool->setMaxThreadCount(8);
}


