#ifndef UPLOADER_H
#define UPLOADER_H

#include "protocol.h"

#include <QObject>

class Uploader : public QObject
{
    Q_OBJECT
public:
    Uploader();
    Uploader(QString strFilePath);

    QString m_strUploadFilePath;
    void start();
public slots:
    void uploadFile();
signals:
    void handlError(const QString& error);
    void uploadPDU(PDU* pdu);
    void finished();

signals:

};

#endif // UPLOADER_H
