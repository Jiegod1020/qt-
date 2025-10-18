#include "client.h"
#include "index.h"
#include "sharefile.h"
#include "ui_sharefile.h"

ShareFile::ShareFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShareFile)
{
    ui->setupUi(this);
}

ShareFile::~ShareFile()
{
    delete ui;
}

void ShareFile::updateFriend_Lw()
{
    ui->listWidget->clear();
    QListWidget* friend_LW = Index::getInstance().getFriend()->getFriend_LW();
    for (int i = 0; i <friend_LW->count(); i++) {
        QListWidgetItem* newItem = new QListWidgetItem(*friend_LW->item(i));
        ui->listWidget->addItem(newItem);
    }
}

void ShareFile::on_allSeleted_PB_clicked()
{
    for (int i = 0; i < ui->listWidget->count(); i++) {
        ui->listWidget->item(i)->setSelected(true);
    }
}

void ShareFile::on_cancelSelected_PB_clicked()
{
    for (int i = 0; i < ui->listWidget->count(); i++) {
        ui->listWidget->item(i)->setSelected(false);
    }
}

void ShareFile::on_ok_PB_clicked()
{
    //获取用户选中的好友
    QList<QListWidgetItem*> pItems = ui->listWidget->selectedItems();
    int friendNum = pItems.size();
    //构建pdu，camsg中放好友名和文件路径，cadata中放当前用户名和好友数量
    PDU* pdu = mkPDU(ENUM_MSG_TYPE_SHARE_FILE_REQUEST, friendNum*32+m_strShareFilePath.toStdString().size()+1);
    memcpy(pdu->caData, Client::getInstance().m_strLoginName.toStdString().c_str(), 32);
    memcpy(pdu->caData+32, &friendNum, 32);

    for (int i = 0; i < friendNum; i++) {
        memcpy(pdu->caMsg+i*32, pItems.at(i)->text().toStdString().c_str(), 32);
    }
    memcpy(pdu->caMsg+friendNum*32,
           m_strShareFilePath.toStdString().c_str(),
           m_strShareFilePath.toStdString().size()
           );
    Client::getInstance().sendMsg(pdu);
}
