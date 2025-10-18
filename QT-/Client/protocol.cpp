#include "protocol.h"
#include "stdlib.h"
#include "string.h"
#include <QDebug>

PDU *mkPDU(uint uiMsgType, uint uiMsgLen)
{
    uint uiPDULen = sizeof(PDU) + uiMsgLen;
    PDU* pdu = (PDU*)malloc(uiPDULen);
    if (pdu == NULL) {
        exit(1);
    }
    memset(pdu, 0, uiPDULen);
    pdu->uiMsgType = uiMsgType;
    pdu->uiMsgLen = uiMsgLen;
    pdu->uiPDULen = uiPDULen;
    return pdu;
}
