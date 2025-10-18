#include "client.h"
#include "onlineuser.h"
#include "protocol.h"
#include "ui_onlineuser.h"

OnlineUser::OnlineUser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OnlineUser)
{
    ui->setupUi(this);
}

OnlineUser::~OnlineUser()
{
    delete ui;
}

void OnlineUser::update_LW(QStringList userList)
{
    ui->listWidget->clear();
    ui->listWidget->addItems(userList);
}

void OnlineUser::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    //获取登录的用户名和选择要添加的用户名
    QString strCurName = Client::getInstance().m_strLoginName;
    QString strTarName = item->text();

    //构建pdu，将两个用户名放入pdu并发送给服务器
    PDU* pdu = mkPDU(ENUM_MSG_TYPE_ADD_FRIEND_REQUEST);
    memcpy(pdu->caData, strCurName.toStdString().c_str(), 32);
    memcpy(pdu->caData+32, strTarName.toStdString().c_str(), 32);
    Client::getInstance().sendMsg(pdu);
}
