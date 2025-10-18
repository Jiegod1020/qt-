#include "mytcpserver.h"
#include "mytcpsocket.h"
#include "operatedb.h"
#include "protocol.h"

MyTcpSocket::MyTcpSocket()
{
    m_pmh = new MsgHandler;
}

void MyTcpSocket::sendMsg(PDU *pdu)
{
    if (pdu == NULL) {
        return;
    }
    this->write((char*)pdu, pdu->uiPDULen);
    qDebug() << "sendMsg uiPDULen" << pdu->uiPDULen
             << "uiMsgLen" << pdu->uiMsgLen
             << "uiMsgType" << pdu->uiMsgType
             << "caData" << pdu->caData
             << "caData+32" << pdu->caData+32
             << "caMsg" << pdu->caMsg;
    free(pdu);
    pdu = NULL;
}

PDU *MyTcpSocket::readPDU()
{
    qDebug() << "readPDU 接收消息的长度" << this->bytesAvailable();

    //读取uiPDULen
    uint uiPDULen = 0;
    this->read((char*)&uiPDULen, sizeof(uint));

    //读取除了uiPDULen以外的内容
    PDU* pdu = mkPDU(0, uiPDULen-sizeof (PDU));
    this->read((char*)pdu+sizeof(uint), uiPDULen - sizeof(uint));
    qDebug() << "readPDU uiPDULen" << pdu->uiPDULen
             << "uiMsgLen" << pdu->uiMsgLen
             << "uiMsgType" << pdu->uiMsgType
             << "caData" << pdu->caData
             << "caData+32" << pdu->caData+32
             << "caMsg" << pdu->caMsg;
    return pdu;
}

PDU *MyTcpSocket::handlePDU(PDU *pdu)
{
    qDebug() << "handlePDU uiPDULen" << pdu->uiPDULen
             << "uiMsgLen" << pdu->uiMsgLen
             << "uiMsgType" << pdu->uiMsgType
             << "caData" << pdu->caData
             << "caData+32" << pdu->caData+32
             << "caMsg" << pdu->caMsg;
    m_pmh->m_pdu = pdu;
    switch (pdu->uiMsgType) {
    case ENUM_MSG_TYPE_REGIST_REQUEST:
        return m_pmh->regist();
    case ENUM_MSG_TYPE_LOGIN_REQUEST:
        return m_pmh->login(m_strLoginName);
    case ENUM_MSG_TYPE_FIND_USER_REQUEST:
        return m_pmh->findUser();
    case ENUM_MSG_TYPE_ONLINE_USER_REQUEST:
        return m_pmh->onlineUser();
    case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST:
        return m_pmh->addFriend();
    case ENUM_MSG_TYPE_ADD_FRIEND_AGREE_REQUEST:
        return m_pmh->addFriendAgree();
    case ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST:
        return m_pmh->flushFriend();
    case ENUM_MSG_TYPE_DEL_FRIEND_REQUEST:
        return m_pmh->delFriend();
    case ENUM_MSG_TYPE_CHAT_REQUEST:
        return m_pmh->chat();
    case ENUM_MSG_TYPE_MKDIR_REQUEST:
        return m_pmh->mkdir();
    case ENUM_MSG_TYPE_FLUSH_FILE_REQUEST:
        return m_pmh->flushFile();
    case ENUM_MSG_TYPE_DEL_DIR_REQUEST:
        return m_pmh->delDir();
    case ENUM_MSG_TYPE_RENAME_FILE_REQUEST:
        return m_pmh->renameFile();
    case ENUM_MSG_TYPE_MV_FILE_REQUEST:
        return m_pmh->mvFile();
    case ENUM_MSG_TYPE_SHARE_FILE_REQUEST:
        return m_pmh->shareFile();
    case ENUM_MSG_TYPE_SHARE_FILE_AGREE_REQUEST:
        return m_pmh->shareFileAgree();
    case ENUM_MSG_TYPE_UPLOAD_FILE_INIT_REQUEST:
        return m_pmh->uploadFileInit();
    case ENUM_MSG_TYPE_UPLOAD_FILE_DATA_REQUEST:
        return m_pmh->uploadFileData();
    default:
        break;
    }
    return NULL;
}

MyTcpSocket::~MyTcpSocket()
{
    delete m_pmh;
    m_pmh = NULL;
}

void MyTcpSocket::recvMsg()
{
    qDebug() << "readPDU 接收消息的长度" << this->bytesAvailable();
    //socket中全部数据读出来
    QByteArray data = readAll();
    //定义一个成员变量buffer处理半包
    buffer.append(data);
    //循环处理buffer中的数据，循环条件为buffer至少为一个PDU大小（才能构建PDU并取出uiPDULen）
    while (buffer.size() >= int(sizeof(PDU))) {
        PDU* pdu = (PDU*)buffer.data();
        if (buffer.size() < int(pdu->uiPDULen)) {
            //判断uiPDULen的大小得到是否为一个完整包
            break;
        }
        PDU* respdu = handlePDU(pdu);
        sendMsg(respdu);
        buffer.remove(0, int(pdu->uiPDULen));//移除处理完的数据
    }
}

void MyTcpSocket::clientOffline()
{
    OperateDB::getInstance().handleOffline(m_strLoginName.toStdString().c_str());
    MyTcpServer::getInstance().deleteSocket(this);
}
