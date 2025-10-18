#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include "protocol.h"

#include <QFile>
#include <QObject>

class MsgHandler
{
public:
    MsgHandler();
    PDU* m_pdu;
    QFile m_fUploadFile;
    qint64 m_iUploadFileSize;
    qint64 m_iReceiveSize;
    PDU* regist();
    PDU* login(QString& strLoginName);
    PDU* findUser();
    PDU* onlineUser();
    PDU* addFriend();
    PDU* addFriendAgree();
    PDU* flushFriend();
    PDU* delFriend();
    PDU* chat();
    PDU* mkdir();
    PDU* flushFile();
    PDU* delDir();
    PDU* renameFile();
    PDU* mvFile();
    PDU* shareFile();
    PDU* shareFileAgree();
    bool copyDir(QString strSrcDir, QString strDestDir);
    PDU* uploadFileInit();
    PDU* uploadFileData();
};

#endif // MSGHANDLER_H
