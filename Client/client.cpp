#include "client.h"
#include "ui_client.h"
#include "protocol.h"
#include "index.h"
#include "uploader.h"

#include <QFile>
#include <QDebug>
#include <QHostAddress>
#include <QMessageBox>

Client::Client(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);
    loadConfig();
    //关联连接信号和槽函数
    connect(&m_tcpSocket, &QTcpSocket::connected, this, &Client::showConnect);
    connect(&m_tcpSocket, &QTcpSocket::readyRead, this, &Client::recvMsg);
    //连接服务器
    m_tcpSocket.connectToHost(QHostAddress(m_strIP), m_usPort);
    m_prh = new ResHandler;
}

Client::~Client()
{
    delete ui;
    delete m_prh;
}

void Client::loadConfig()
{
    //读写文件需要创建一个QFile文件对象，参数为路径
    QFile file(":/socket.config");
    //打开文件，参数为打开方式
    if (file.open(QIODevice::ReadOnly)) {
        //读取文件全部内容
        QByteArray baData = file.readAll();
        QString strData = QString(baData);
        QStringList strList = strData.split("\r\n");//拆分文件，参数为按什么拆分
        m_strIP = strList.at(0);
        m_usPort = strList.at(1).toUShort();
        qDebug() << "loadConfig m_strIP" << m_strIP
                 << "m_usPort" << m_usPort;
        file.close();
    } else {
        qDebug() << "loadConfig failed";
    }
}

Client &Client::getInstance()
{
    static Client instance;
    return instance;
}

void Client::sendMsg(PDU *pdu)
{
    m_tcpSocket.write((char*)pdu, pdu->uiPDULen);
    qDebug() << "sendMsg uiPDULen" << pdu->uiPDULen
             << "uiMsgLen" << pdu->uiMsgLen
             << "uiMsgType" << pdu->uiMsgType
             << "caData" << pdu->caData
             << "caData+32" << pdu->caData+32
             << "caMsg" << pdu->caMsg;
    free(pdu);
    pdu = NULL;
}

PDU *Client::readPDU()
{
    qDebug() << "readPDU 接收消息的长度" << m_tcpSocket.bytesAvailable();

    //读取uiPDULen
    uint uiPDULen = 0;
    m_tcpSocket.read((char*)&uiPDULen, sizeof(uint));

    //读取除了uiPDULen以外的内容
    PDU* pdu = mkPDU(0, uiPDULen-sizeof (PDU));
    m_tcpSocket.read((char*)pdu+sizeof(uint), uiPDULen - sizeof(uint));
    qDebug() << "readPDU uiPDULen" << pdu->uiPDULen
             << "uiMsgLen" << pdu->uiMsgLen
             << "uiMsgType" << pdu->uiMsgType
             << "caData" << pdu->caData
             << "caData+32" << pdu->caData+32
             << "caMsg" << pdu->caMsg;
    return pdu;
}

void Client::handlePDU(PDU *pdu)
{
    qDebug() << "handlePDU uiPDULen" << pdu->uiPDULen
             << "uiMsgLen" << pdu->uiMsgLen
             << "uiMsgType" << pdu->uiMsgType
             << "caData" << pdu->caData
             << "caData+32" << pdu->caData+32
             << "caMsg" << pdu->caMsg;
    m_prh->m_pdu = pdu;
    switch (pdu->uiMsgType) {
    case ENUM_MSG_TYPE_REGIST_RESPOND:
        return m_prh->regist();
    case ENUM_MSG_TYPE_LOGIN_RESPOND:
        return m_prh->login(m_strLoginName);
    case ENUM_MSG_TYPE_FIND_USER_RESPOND:
        return m_prh->findUser();
    case ENUM_MSG_TYPE_ONLINE_USER_RESPOND:
        return m_prh->onlineUser(m_strLoginName);
    case ENUM_MSG_TYPE_ADD_FRIEND_RESPOND:
        return m_prh->addFriend();
    case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST:
        return m_prh->addFriendResend();
    case ENUM_MSG_TYPE_ADD_FRIEND_AGREE_RESPOND:
        return m_prh->addFriendAgree();
    case ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND:
        return m_prh->flushFriend();
    case ENUM_MSG_TYPE_DEL_FRIEND_RESPOND:
        return m_prh->delFriend();
    case ENUM_MSG_TYPE_CHAT_REQUEST:
        return m_prh->chat();
    case ENUM_MSG_TYPE_MKDIR_RESPOND:
        return m_prh->mkdir();
    case ENUM_MSG_TYPE_FLUSH_FILE_RESPOND:
        return m_prh->flushFile();
    case ENUM_MSG_TYPE_DEL_DIR_RESPOND:
        return m_prh->delDir();
    case ENUM_MSG_TYPE_RENAME_FILE_RESPOND:
        return m_prh->renameFile();
    case ENUM_MSG_TYPE_MV_FILE_RESPOND:
        return m_prh->mvFile();
    case ENUM_MSG_TYPE_SHARE_FILE_RESPOND:
        return m_prh->shareFile();
    case ENUM_MSG_TYPE_SHARE_FILE_REQUEST:
        return m_prh->shareFileResend();
    case ENUM_MSG_TYPE_SHARE_FILE_AGREE_RESPOND:
        return m_prh->shareFileAgree();
    case ENUM_MSG_TYPE_UPLOAD_FILE_INIT_RESPOND:
        return m_prh->uploadFileInit();
    case ENUM_MSG_TYPE_UPLOAD_FILE_DATA_RESPOND:
        return m_prh->uploadFileData();
    default:
        break;
    }
}

void Client::startUpload()
{
    Uploader* uploader = new Uploader(Index::getInstance().getFile()->m_strUploadFilePath);
    connect(uploader, &Uploader::handlError, this, &Client::uploadError);
    connect(uploader, &Uploader::uploadPDU, this, &Client::sendMsg);
    uploader->start();
}

void Client::showConnect()
{
    qDebug() << "连接服务器成功";
}

void Client::recvMsg()
{

    qDebug() << "readPDU 接收消息的长度" << m_tcpSocket.bytesAvailable();
    //socket中全部数据读出来
    QByteArray data = m_tcpSocket.readAll();
    //定义一个成员变量buffer处理半包
    buffer.append(data);
    //循环处理buffer中的数据，循环条件为buffer至少为一个PDU大小（才能构建PDU并取出uiPDULen）
    while (buffer.size() >= int(sizeof(PDU))) {
        PDU* pdu = (PDU*)buffer.data();
        if (buffer.size() < int(pdu->uiPDULen)) {
            //判断uiPDULen的大小得到是否为一个完整包
            break;
        }
        handlePDU(pdu);
        buffer.remove(0, int(pdu->uiPDULen));//移除处理完的数据
    }
}

void Client::uploadError(const QString &error)
{
    QMessageBox::information(&Index::getInstance(), "上传文件", error);
}


void Client::on_regist_PB_clicked()
{
    //获取用户输入的用户名和密码
    QString strName = ui->name_LE->text();
    QString strPwd = ui->pwd_LE->text();
    if (strName.toStdString().size() > 32 || strPwd.toStdString().size() > 32
            || strName.isEmpty() || strPwd.isEmpty()) {
        QMessageBox::information(this, "注册", "用户名或密码长度非法");
        return;
    }

    //构建pdu，用户名和密码放入caData
    PDU* pdu = mkPDU(ENUM_MSG_TYPE_REGIST_REQUEST);
    memcpy(pdu->caData, strName.toStdString().c_str(), 32);
    memcpy(pdu->caData+32, strPwd.toStdString().c_str(), 32);

    sendMsg(pdu);
}

void Client::on_login_PB_clicked()
{
    //获取用户名和密码
    QString strName = ui->name_LE->text();
    QString strPwd = ui->pwd_LE->text();
    //记录用户名
    m_strLoginName = strName;

    //构建pdu，将用户名和密码放入caData
    PDU* pdu = mkPDU(ENUM_MSG_TYPE_LOGIN_REQUEST);
    memcpy(pdu->caData, strName.toStdString().c_str(), 32);
    memcpy(pdu->caData+32, strPwd.toStdString().c_str(), 32);

    sendMsg(pdu);
}
