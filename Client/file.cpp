#include "client.h"
#include "file.h"
#include "ui_file.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

File::File(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::File)
{
    ui->setupUi(this);
    m_strCurPath = QString("./filesys/%1").arg(Client::getInstance().m_strLoginName);
    m_strUserPath = m_strCurPath;
    m_pShareFile = new ShareFile;
    flushFile();
}

File::~File()
{
    delete ui;
    delete m_pShareFile;
}

void File::updateFileList(QList<FileInfo *> pFileList)
{
    //每次刷新，释放原来存的文件信息
    foreach (FileInfo* pFileInfo, m_pFileInfoList) {
        delete pFileInfo;
    }
    m_pFileInfoList.clear();
    m_pFileInfoList = pFileList;
    //清空列表框，遍历文件信息添加到列表框
    ui->listWidget->clear();
    foreach(FileInfo* pFileInfo, pFileList) {
        QListWidgetItem* pItem = new QListWidgetItem;
        if (pFileInfo->iFileType == 0) {
            pItem->setIcon(QIcon(QPixmap(":/dir.png")));
        } else {
            pItem->setIcon(QIcon(QPixmap(":/file.png")));
        }
        pItem->setText(pFileInfo->caName);
        ui->listWidget->addItem(pItem);
    }
}

void File::uploadFile()
{
    //创建文件对象并打开
    QFile file(m_strUploadFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "上传文件", "打开文件失败");
        return;
    }
    //构建pdu，每次发送4096
    PDU * pdu = mkPDU(ENUM_MSG_TYPE_UPLOAD_FILE_DATA_REQUEST, 40960);
    //循环读取文件内容并发送
    while (true) {
        int ret = file.read(pdu->caMsg, 40960);
        if (ret == 0) {
            break;
        }
        if (ret < 0) {
            QMessageBox::warning(this, "上传文件", "读取文件失败");
            break;
        }
        //最后一次读取长度可能不是4096，需要更新
        pdu->uiMsgLen = ret;
        pdu->uiPDULen = ret + sizeof(PDU);
        Client::getInstance().m_tcpSocket.write((char*)pdu, pdu->uiPDULen);
        qDebug() << "sendMsg uiPDULen" << pdu->uiPDULen
                 << "uiMsgLen" << pdu->uiMsgLen
                 << "uiMsgType" << pdu->uiMsgType
                 << "caData" << pdu->caData
                 << "caData+32" << pdu->caData+32;
    }
    file.close();
    free(pdu);
    pdu = NULL;
}

void File::flushFile()
{
    PDU* pdu = mkPDU(ENUM_MSG_TYPE_FLUSH_FILE_REQUEST, m_strCurPath.toStdString().size()+1);
    memcpy(pdu->caMsg, m_strCurPath.toStdString().c_str(), m_strCurPath.toStdString().size());
    Client::getInstance().sendMsg(pdu);
}

void File::on_mkdir_PB_clicked()
{
    QString strNewDir = QInputDialog::getText(this, "新建文件夹", "新建文件夹名：");
    if (strNewDir.isEmpty() || strNewDir.toStdString().size() > 32) {
        QMessageBox::information(this, "新建文件夹", "文件夹名长度非法");
        return;
    }
    PDU* pdu = mkPDU(ENUM_MSG_TYPE_MKDIR_REQUEST, m_strCurPath.toStdString().size()+1);
    memcpy(pdu->caData, strNewDir.toStdString().c_str(), 32);
    memcpy(pdu->caMsg, m_strCurPath.toStdString().c_str(), m_strCurPath.toStdString().size());
    Client::getInstance().sendMsg(pdu);
}

void File::on_flush_PB_clicked()
{
    flushFile();
}

void File::on_deldir_PB_clicked()
{
    //获取用户选择的文件并判空
    QListWidgetItem* pItem = ui->listWidget->currentItem();
    if (pItem == NULL) {
        return;
    }
    QString strDelFileName = pItem->text();
    //判断选择的文件是否为文件夹
    foreach(FileInfo* pFileInfo, m_pFileInfoList) {
        if (strDelFileName == pFileInfo->caName && pFileInfo->iFileType != 0) {
            QMessageBox::warning(this, "删除文件夹", "选择的不是文件夹");
            return;
        }
    }
    //让用户确认是否删除
    int ret = QMessageBox::question(this, "删除文件夹", QString("是否确定删除文件夹 %1").arg(strDelFileName));
    if (ret != QMessageBox::Yes) {
        return;
    }
    //拼接完整路径并放入pdu，发送
    QString strPath = QString("%1/%2").arg(m_strCurPath).arg(strDelFileName);
    PDU* pdu = mkPDU(ENUM_MSG_TYPE_DEL_DIR_REQUEST, strPath.toStdString().size()+1);
    memcpy(pdu->caMsg, strPath.toStdString().c_str(), strPath.toStdString().size());
    Client::getInstance().sendMsg(pdu);
}

void File::on_rename_PB_clicked()
{
    //获取用户选择的文件并判空
    QListWidgetItem* pItem = ui->listWidget->currentItem();
    if (pItem == NULL) {
        return;
    }
    QString strOldFileName = pItem->text();
    //输入框获取新文件名，并判断长度
    QString strNewFileName = QInputDialog::getText(this, "重命名文件", "新文件名");
    if (strNewFileName.isEmpty() || strNewFileName.toStdString().size() > 32) {
        QMessageBox::information(this, "重命名文件", "文件名长度非法");
        return;
    }
    //构建pdu，新旧文件名放cadata，当前路径放camsg
    PDU* pdu = mkPDU(ENUM_MSG_TYPE_RENAME_FILE_REQUEST, m_strCurPath.toStdString().size()+1);
    memcpy(pdu->caData, strOldFileName.toStdString().c_str(), 32);
    memcpy(pdu->caData+32, strNewFileName.toStdString().c_str(), 32);
    memcpy(pdu->caMsg, m_strCurPath.toStdString().c_str(), m_strCurPath.toStdString().size());
    Client::getInstance().sendMsg(pdu);
}

void File::on_return_PB_clicked()
{
    if (m_strCurPath == m_strUserPath) {
        return;
    }
    int index = m_strCurPath.lastIndexOf('/');
    m_strCurPath.remove(index, m_strCurPath.toStdString().size() - index);
    flushFile();
}

void File::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString strDirName = item->text();
    //判断选择的文件是否为文件夹
    foreach(FileInfo* pFileInfo, m_pFileInfoList) {
        if (strDirName == pFileInfo->caName && pFileInfo->iFileType != 0) {
            QMessageBox::warning(this, "提示", "选择的不是文件夹");
            return;
        }
    }
    m_strCurPath = QString("%1/%2").arg(m_strCurPath).arg(strDirName);
    flushFile();
}

void File::on_mv_PB_clicked()
{
    //移动文件按钮
    if (ui->mv_PB->text() == "移动文件") {
        //获取用户选择的文件并判空
        QListWidgetItem* pItem = ui->listWidget->currentItem();
        if (pItem == NULL) {
            return;
        }
        //存移动文件名和完整路径
        m_strMvFileName = pItem->text();
        m_strMvFilePath = QString("%1/%2").arg(m_strCurPath).arg(m_strMvFileName);
        QMessageBox::information(this, "移动文件", "请选择要移动到的目录");
        //修改按钮文字
        ui->mv_PB->setText("确认/取消");
        return;
    }
    //修改按钮文字
    ui->mv_PB->setText("移动文件");
    //获取目标路径
    QListWidgetItem* pItem = ui->listWidget->currentItem();
    QString strTarPath;
    if (pItem == NULL) {
        //没有选择目录，使用当前路径
        strTarPath = QString("%1/%2").arg(m_strCurPath).arg(m_strMvFileName);
    } else {
        //选择目录，使用当前路径+选择的目录为目标路径
        foreach(FileInfo* pFileInfo, m_pFileInfoList) {
            if (pItem->text() == pFileInfo->caName && pFileInfo->iFileType != 0) {
                QMessageBox::warning(this, "提示", "选择的不是文件夹");
                return;
            }
        }
        strTarPath = QString("%1/%2/%3").arg(m_strCurPath).arg(pItem->text()).arg(m_strMvFileName);
    }
    //用户确认路径
    int ret = QMessageBox::information(this, "移动文件", QString("新的路径是否为：%1").arg(strTarPath), "确认", "取消");
    if (ret !=0 ) {
        return;
    }
    //两个路径放入camsg，方便服务器从camsg中取出路径，将长度放入cadata
    int srcLen = m_strMvFilePath.toStdString().size();
    int tarLen = strTarPath.toStdString().size();
    PDU* pdu = mkPDU(ENUM_MSG_TYPE_MV_FILE_REQUEST, srcLen+tarLen+1);
    memcpy(pdu->caData, &srcLen, 32);
    memcpy(pdu->caData+32, &tarLen, 32);

    memcpy(pdu->caMsg, m_strMvFilePath.toStdString().c_str(), srcLen);
    memcpy(pdu->caMsg+srcLen, strTarPath.toStdString().c_str(), tarLen);
    Client::getInstance().sendMsg(pdu);
}

void File::on_share_PB_clicked()
{
    QListWidgetItem* pItem = ui->listWidget->currentItem();
    if (pItem == NULL) {
        QMessageBox::information(this, "分享文件", "请选择要分享的文件");
        return;
    }
    m_pShareFile->m_strShareFilePath = m_strCurPath + '/' + pItem->text();
    m_pShareFile->updateFriend_Lw();
    if (m_pShareFile->isHidden()) {
        m_pShareFile->show();
    }
}

void File::on_upload_PB_clicked()
{
    //获取上传文件的路径，并作为成员变量存下来
    m_strUploadFilePath.clear();
    m_strUploadFilePath = QFileDialog::getOpenFileName();
    if (m_strUploadFilePath.isEmpty()) {
        return;
    }
    //构建pdu，文件名和文件大小放入cadata，存储路径放入camsg
    PDU* pdu = mkPDU(ENUM_MSG_TYPE_UPLOAD_FILE_INIT_REQUEST, m_strCurPath.toStdString().size()+1);
    //获取路径中的文件名
    int index = m_strUploadFilePath.lastIndexOf('/');
    QString strFileName = m_strUploadFilePath.right(m_strUploadFilePath.size() - index - 1);
    memcpy(pdu->caData, strFileName.toStdString().c_str(), 32);
    //获取文件大小
    QFile file(m_strUploadFilePath);
    qint64 fileSize = file.size();
    memcpy(pdu->caData+32, &fileSize, sizeof(qint64));
    memcpy(pdu->caMsg, m_strCurPath.toStdString().c_str(), m_strCurPath.toStdString().size());
    Client::getInstance().sendMsg(pdu);
}



