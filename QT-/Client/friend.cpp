#include "client.h"
#include "friend.h"
#include "protocol.h"
#include "ui_friend.h"

#include <QInputDialog>
#include <QMessageBox>

Friend::Friend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Friend)
{
    ui->setupUi(this);
    m_pOnlineUser = new OnlineUser;
    m_pChat = new Chat;
    flushFriend();
}

Friend::~Friend()
{
    delete ui;
    delete m_pOnlineUser;
    delete m_pChat;
}

void Friend::update_LW(QStringList userList)
{
    ui->listWidget->clear();
    ui->listWidget->addItems(userList);
}

void Friend::flushFriend()
{
    PDU* pdu = mkPDU(ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST);
    memcpy(pdu->caData, Client::getInstance().m_strLoginName.toStdString().c_str(), 32);
    Client::getInstance().sendMsg(pdu);
}

QListWidget *Friend::getFriend_LW()
{
    return ui->listWidget;
}

void Friend::on_findUser_PB_clicked()
{
    QString strName = QInputDialog::getText(this, "搜索", "用户名：");
    if (strName.isEmpty()) {
        return;
    }
    if (strName.toStdString().size() > 32) {
        QMessageBox::information(this, "搜索", "查找的用户名长度非法");
        return;
    }
    PDU* pdu = mkPDU(ENUM_MSG_TYPE_FIND_USER_REQUEST);
    memcpy(pdu->caData, strName.toStdString().c_str(), 32);
    Client::getInstance().sendMsg(pdu);
}

void Friend::on_onlineUser_PB_clicked()
{
    if (m_pOnlineUser->isHidden()) {
        m_pOnlineUser->show();
    }
    PDU* pdu = mkPDU(ENUM_MSG_TYPE_ONLINE_USER_REQUEST);
    Client::getInstance().sendMsg(pdu);
}

void Friend::on_flush_PB_clicked()
{
    flushFriend();
}

void Friend::on_del_PB_clicked()
{
    QListWidgetItem* pItem = ui->listWidget->currentItem();
    if (!pItem) {
        return;
    }
    QString strTarName = pItem->text();
    int ret = QMessageBox::question(this, "删除好友", QString("是否确认删除好友 '%1'?").arg(strTarName));
    if (ret != QMessageBox::Yes) {
        return;
    }
    PDU* pdu = mkPDU(ENUM_MSG_TYPE_DEL_FRIEND_REQUEST);
    memcpy(pdu->caData, Client::getInstance().m_strLoginName.toStdString().c_str(), 32);
    memcpy(pdu->caData+32, strTarName.toStdString().c_str(), 32);
    Client::getInstance().sendMsg(pdu);
}

void Friend::on_chat_PB_clicked()
{
    QListWidgetItem* pItem = ui->listWidget->currentItem();
    if (!pItem) {
        return;
    }
    m_pChat->m_strChatName = pItem->text();
    if (m_pChat->isHidden()) {
        m_pChat->setWindowTitle(QString("to: %1").arg(m_pChat->m_strChatName));
        m_pChat->show();
    }
}
