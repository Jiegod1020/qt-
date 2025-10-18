#include "protocol.h"
#include "uploader.h"

#include <QFile>
#include <QThread>

Uploader::Uploader()
{

}

Uploader::Uploader(QString strFilePath)
{
    m_strUploadFilePath = strFilePath;
}

void Uploader::uploadFile()
{
    //创建文件对象并打开
    QFile file(m_strUploadFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        emit handlError("打开文件失败");
        emit finished();
        return;
    }

    //循环读取文件内容并发送
    while (true) {
        //构建pdu，每次发送4096
        PDU * pdu = mkPDU(ENUM_MSG_TYPE_UPLOAD_FILE_DATA_REQUEST, 40960);
        int ret = file.read(pdu->caMsg, 40960);
        if (ret == 0) {
            break;
        }
        if (ret < 0) {
            emit handlError("读取文件失败");
            break;
        }
        //最后一次读取长度可能不是4096，需要更新
        pdu->uiMsgLen = ret;
        pdu->uiPDULen = ret + sizeof(PDU);
        emit uploadPDU(pdu);
    }
    file.close();
    emit finished();
}

void Uploader::start()
{
    //创建线程对象
    QThread* thread = new QThread;
    //当前对象移交给线程
    this->moveToThread(thread);
    //线程执行后，当前对象执行uploadFile函数
    connect(thread, &QThread::started, this, &Uploader::uploadFile);
    //uploadFile运行完，线程退出
    connect(this, &Uploader::finished, thread, &QThread::quit);
    //线程结束，释放线程对象
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    //线程开始执行
    thread->start();
}
