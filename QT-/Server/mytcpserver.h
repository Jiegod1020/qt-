#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QThreadPool>
#include "mytcpsocket.h"

class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    static MyTcpServer& getInstance();
    void incomingConnection(qintptr handle) override;
    void deleteSocket(MyTcpSocket* mysocket);
    void resend(char* caTarName, PDU* pdu);
    QThreadPool threadPool;
private:
    MyTcpServer();
    MyTcpServer(const MyTcpServer& instance) = delete;
    MyTcpServer& operator=(const MyTcpServer&) = delete;
    QList<MyTcpSocket*> m_tcpSocketList;
};

#endif // MYTCPSERVER_H
