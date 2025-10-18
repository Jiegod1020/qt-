#include "client.h"
#include "index.h"
#include "reshandler.h"
#include "uploader.h"

#include <QMessageBox>

ResHandler::ResHandler()
{

}

void ResHandler::regist()
{
    bool ret;
    memcpy(&ret, m_pdu->caData, sizeof(bool));
    if (ret) {
        QMessageBox::information(&Client::getInstance(), "注册", "注册成功");
    } else {
        QMessageBox::information(&Client::getInstance(), "注册", "注册失败");
    }
}

void ResHandler::login(QString strLoginName)
{
    bool ret;
    memcpy(&ret, m_pdu->caData, sizeof(bool));
    if (ret) {
        Index::getInstance().show();
        Index::getInstance().setWindowTitle(strLoginName);
        Client::getInstance().hide();
    } else {
        QMessageBox::information(&Client::getInstance(), "登录", "登录失败");
    }
}

void ResHandler::findUser()
{
    char caName[32] = {'\0'};
    memcpy(caName, m_pdu->caData, 32);
    if (Client::getInstance().m_strLoginName == caName) {
        QMessageBox::information(&Index::getInstance(), "搜索", "这个大帅哥竟然是我自己");
        return;
    }
    int ret;
    memcpy(&ret, m_pdu->caData+32, sizeof(int));
    if (ret == -1) {
        QMessageBox::information(&Index::getInstance(), "搜索", "查找用户失败");
    } else if (ret == 0) {
        QMessageBox::information(&Index::getInstance(), "搜索", QString("'%1' 不在线").arg(caName));
    } else if (ret == 1) {
        int ret = QMessageBox::information(&Index::getInstance(), "搜索", QString("'%1' 在线").arg(caName), "添加好友", "取消");
        if (ret == 0) {
            //获取登录的用户名和选择要添加的用户名
            QString strCurName = Client::getInstance().m_strLoginName;
            QString strTarName = QString(caName);

            //构建pdu，将两个用户名放入pdu并发送给服务器
            PDU* pdu = mkPDU(ENUM_MSG_TYPE_ADD_FRIEND_REQUEST);
            memcpy(pdu->caData, strCurName.toStdString().c_str(), 32);
            memcpy(pdu->caData+32, strTarName.toStdString().c_str(), 32);
            Client::getInstance().sendMsg(pdu);
        }
    } else if (ret == 2) {
        QMessageBox::information(&Index::getInstance(), "搜索", QString("'%1' 不存在").arg(caName));
    }
}

void ResHandler::onlineUser(QString strLoginName)
{
    uint uiSize = m_pdu->uiMsgLen/32;
    QStringList userList;
    char caTmp[32] = {'\0'};
    for (uint i = 0; i < uiSize; i++) {
        memcpy(caTmp, m_pdu->caMsg+i*32, 32);
        if (QString(caTmp) == strLoginName) {
            continue;
        }
        userList.append(QString(caTmp));
    }
    Index::getInstance().getFriend()->m_pOnlineUser->update_LW(userList);
}

void ResHandler::addFriend()
{
    int ret;
    memcpy(&ret, m_pdu->caData, sizeof(int));
    if (ret == -1) {
        QMessageBox::information(&Index::getInstance(), "搜索", "添加好友错误");
    } else if (ret == 0) {
        QMessageBox::information(&Index::getInstance(), "搜索", "该用户不在线");
    } else if (ret == -2) {
        QMessageBox::information(&Index::getInstance(), "搜索", "该用户已经是你的好友");
    }
}

void ResHandler::addFriendResend()
{
    char caName[32] = {'\0'};
    memcpy(caName, m_pdu->caData, 32);
    int ret = QMessageBox::question(&Index::getInstance(), "添加好友", QString("是否同意 %1 的添加好友请求？").arg(caName));
    if (ret != QMessageBox::Yes) {
        return;
    }
    PDU* respdu = mkPDU(ENUM_MSG_TYPE_ADD_FRIEND_AGREE_REQUEST);
    memcpy(respdu->caData, m_pdu->caData, 64);
    Client::getInstance().sendMsg(respdu);
}

void ResHandler::addFriendAgree()
{
    bool ret;
    memcpy(&ret, m_pdu->caData, sizeof(bool));
    if (ret) {
        QMessageBox::information(&Index::getInstance(), "添加好友", "添加好友成功");
        Index::getInstance().getFriend()->flushFriend();
    } else {
        QMessageBox::information(&Index::getInstance(), "添加好友", "添加好友失败");
    }
}

void ResHandler::flushFriend()
{
    uint uiSize = m_pdu->uiMsgLen/32;
    QStringList userList;
    char caTmp[32] = {'\0'};
    for (uint i = 0; i < uiSize; i++) {
        memcpy(caTmp, m_pdu->caMsg+i*32, 32);
        userList.append(QString(caTmp));
    }
    Index::getInstance().getFriend()->update_LW(userList);
}

void ResHandler::delFriend()
{
    bool ret;
    memcpy(&ret, m_pdu->caData, sizeof(bool));
    if (!ret) {
        QMessageBox::information(&Index::getInstance(), "删除好友", "删除好友失败");
    }
    Index::getInstance().getFriend()->flushFriend();
}

void ResHandler::chat()
{
    Chat* c = Index::getInstance().getFriend()->m_pChat;
    if (c->isHidden()) {
        c->show();
    }
    char caChatName[32] = {'\0'};
    memcpy(caChatName, m_pdu->caData, 32);
    c->updateShow_TE(QString("%1: %2").arg(caChatName).arg(m_pdu->caMsg));
    c->setWindowTitle(QString("to: %1").arg(caChatName));
    c->m_strChatName = caChatName;
}

void ResHandler::mkdir()
{
    bool ret;
    memcpy(&ret, m_pdu->caData, sizeof(bool));
    if (ret) {
        Index::getInstance().getFile()->flushFile();
    } else {
        QMessageBox::information(&Index::getInstance(), "创建文件夹", "创建文件夹失败");
    }
}

void ResHandler::flushFile()
{
    int iCount = m_pdu->uiMsgLen / sizeof(FileInfo);
    qDebug() << "file count" << iCount;
    QList<FileInfo*> pFileList;
    for (int i = 0; i < iCount; i++) {
        FileInfo* pFileInfo = new FileInfo;
        memcpy(pFileInfo, m_pdu->caMsg+i*sizeof(FileInfo), sizeof(FileInfo));
        pFileList.append(pFileInfo);
    }
    Index::getInstance().getFile()->updateFileList(pFileList);
}

void ResHandler::delDir()
{
    bool ret;
    memcpy(&ret, m_pdu->caData, sizeof(bool));
    if (ret) {
        Index::getInstance().getFile()->flushFile();
    } else {
        QMessageBox::information(&Index::getInstance(), "删除文件夹", "删除文件夹失败");
    }
}

void ResHandler::renameFile()
{
    bool ret;
    memcpy(&ret, m_pdu->caData, sizeof(bool));
    if (ret) {
        Index::getInstance().getFile()->flushFile();
    } else {
        QMessageBox::information(&Index::getInstance(), "重命名文件", "重命名文件失败");
    }
}

void ResHandler::mvFile()
{
    bool ret;
    memcpy(&ret, m_pdu->caData, sizeof(bool));
    if (ret) {
        Index::getInstance().getFile()->flushFile();
    } else {
        QMessageBox::information(&Index::getInstance(), "移动文件", "移动文件失败");
    }
}

void ResHandler::shareFile()
{
    QMessageBox::information(&Index::getInstance(), "分享文件", "文件已分享");
}

void ResHandler::shareFileResend()
{
    QString strFilePath = QString(m_pdu->caMsg);
    int index = strFilePath.lastIndexOf('/');
    QString strFileName = strFilePath.right(strFilePath.size() - index - 1);

    int ret = QMessageBox::question(
                &Index::getInstance(),
                "分享文件",
                QString("%1 分享文件 %2\n是否接收？").arg(m_pdu->caData).arg(strFileName));
    if (ret != QMessageBox::Yes) {
        return;
    }
    PDU* pdu = mkPDU(ENUM_MSG_TYPE_SHARE_FILE_AGREE_REQUEST, m_pdu->uiMsgLen);
    memcpy(pdu->caData, Client::getInstance().m_strLoginName.toStdString().c_str(), 32);
    memcpy(pdu->caMsg, m_pdu->caMsg, pdu->uiMsgLen);
    Client::getInstance().sendMsg(pdu);
}

void ResHandler::shareFileAgree()
{
    bool ret;
    memcpy(&ret, m_pdu->caData, sizeof(bool));
    if (ret) {
        QMessageBox::information(&Index::getInstance(), "分享文件", "分享文件成功");
        Index::getInstance().getFile()->flushFile();
    } else {
        QMessageBox::information(&Index::getInstance(), "分享文件", "分享文件失败");
    }
}

void ResHandler::uploadFileInit()
{
    bool ret;
    memcpy(&ret, m_pdu->caData, sizeof(bool));
    if (ret) {
       Client::getInstance().startUpload();
    } else {
        QMessageBox::information(&Index::getInstance(), "上传文件", "上传文件初始化失败");
    }
}

void ResHandler::uploadFileData()
{
    bool ret;
    memcpy(&ret, m_pdu->caData, sizeof(bool));
    if (ret) {
        QMessageBox::information(&Index::getInstance(), "上传文件", "上传文件成功");
    } else {
        QMessageBox::information(&Index::getInstance(), "上传文件", "上传文件初始化失败");
    }
    Index::getInstance().getFile()->flushFile();
}
