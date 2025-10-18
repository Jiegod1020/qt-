#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QWidget>
#include "protocol.h"
#include "reshandler.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Client; }
QT_END_NAMESPACE

class Client : public QWidget
{
    Q_OBJECT

public:
    ~Client();
    void loadConfig();
    QString m_strIP;
    quint16 m_usPort;
    QTcpSocket m_tcpSocket;
    QString m_strLoginName;
    ResHandler* m_prh;
    static Client& getInstance();
    PDU* readPDU();
    void handlePDU(PDU* pdu);
    QByteArray buffer;
    void startUpload();
public slots:
    void showConnect();
    void recvMsg();
    void uploadError(const QString& error);
    void sendMsg(PDU* pdu);

private slots:
    void on_regist_PB_clicked();

    void on_login_PB_clicked();

private:
    Ui::Client *ui;
    Client(QWidget *parent = nullptr);
    Client(const Client& instance) = delete;
    Client& operator=(const Client&) = delete;
};
#endif // CLIENT_H
