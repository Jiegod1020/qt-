#include "clienttask.h"
#include "mytcpserver.h"

MyTcpServer::MyTcpServer()
{
    threadPool.setMaxThreadCount(8);
}

MyTcpServer &MyTcpServer::getInstance()
{
    static MyTcpServer instance;
    return instance;
}

void MyTcpServer::incomingConnection(qintptr handle)
{
    qDebug() << "新客户端连接";
    MyTcpSocket* pTcpSocket = new MyTcpSocket;
    pTcpSocket->setSocketDescriptor(handle);
    m_tcpSocketList.append(pTcpSocket);
    ClientTask* task = new ClientTask(pTcpSocket);
    threadPool.start(task);
}

void MyTcpServer::deleteSocket(MyTcpSocket *mysocket)
{
    m_tcpSocketList.removeOne(mysocket);
    mysocket->deleteLater();
    mysocket = NULL;
}

void MyTcpServer::resend(char *caTarName, PDU *pdu)
{
    if (caTarName == NULL || pdu == NULL) {
        return;
    }
    for (int i = 0; i < m_tcpSocketList.size(); i++) {
        if (caTarName == m_tcpSocketList.at(i)->m_strLoginName) {
            m_tcpSocketList.at(i)->write((char*)pdu, pdu->uiPDULen);
            qDebug() << "resend uiPDULen" << pdu->uiPDULen
                     << "uiMsgLen" << pdu->uiMsgLen
                     << "uiMsgType" << pdu->uiMsgType
                     << "caData" << pdu->caData
                     << "caData+32" << pdu->caData+32
                     << "caMsg" << pdu->caMsg;
        }
    }
}
