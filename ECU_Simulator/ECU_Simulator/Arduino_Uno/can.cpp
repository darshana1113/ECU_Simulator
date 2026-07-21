#include "can.h"

MCP_CAN CAN(CAN_CS);        // create a CAN obj and class MCP_CAN

bool CAN_Init()
{
    while (CAN.begin(MCP_ANY, CAN_SPEED, CAN_CLOCK) != CAN_OK)
    {
        Serial.println("CAN Initialization Failed");
        delay(1000);
    }

    CAN.setMode(MCP_NORMAL);

    Serial.println("CAN Initialized");

    return true;
}

bool CAN_Send(unsigned long id, byte len, byte *data)
{
    return (CAN.sendMsgBuf(id, 0, len, data) == CAN_OK);  // 0 : std 11-bit CAN-ID  , 1 : extended 29-bit CAN-ID
}

bool CAN_Receive(unsigned long *id, byte *len, byte *data)
{
    if (CAN.checkReceive() == CAN_MSGAVAIL)
    {
        CAN.readMsgBuf(id, len, data);
        return true;
    }

    return false;
}