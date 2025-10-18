#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QObject>
#include <QTcpSocket>

#include "msghandler.h"
#include "protocol.h"

class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    MyTcpSocket();
    void sendMsg(PDU* pdu);
    PDU* readPDU();
    PDU* handlePDU(PDU* pdu);
    MsgHandler* m_pmh;
    QString m_strLoginName;
    ~MyTcpSocket();
    QByteArray buffer;
public slots:
    void recvMsg();//接收消息的处理函数
    void clientOffline();
};

#endif // MYTCPSOCKET_H
