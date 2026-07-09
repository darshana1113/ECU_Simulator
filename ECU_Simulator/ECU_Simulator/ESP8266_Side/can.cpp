#include "can.h"         // includes all declarations from can.h , now it nows the CAN_speed , CAN_Clock , CAN_CS

MCP_CAN CAN(CAN_CS);          // constructor receives CAN_CS


//------------------------------------
// Initialize MCP2515


bool CAN_Init()             // can initialization 
{
    while (CAN.begin(MCP_ANY, CAN_SPEED, CAN_CLOCK) != CAN_OK)
    {
        Serial.println("CAN Initialization Failed");
        delay(1000);
    }

    CAN.setMode(MCP_NORMAL);    // this mode is used to start communicating on the real CAN bus. without this can module stay in configuration mode and never transmit message .

    Serial.println("CAN Initialized Successfully");

    return true;
}

//------------------------------------
// Receive CAN Frame

               //(Store CAN id , msg len , actual data) passing the addresses of the variable 
bool CAN_Receive(unsigned long *id, byte *len, byte *data)   // This fun will check msg had arrived or not .
{
    if (CAN.checkReceive() == CAN_MSGAVAIL)   // store receives msg in buffer 
    {
        CAN.readMsgBuf(id, len, data);
        return true;
    }                              // inside this fun we store value when fun finishes values are lost.

    return false;
}

//------------------------------------
// Send CAN Frame

bool CAN_Send(unsigned long id, byte len, byte *data) // our data is in array format , array automatically behaves like pointers when passes to functions.
{
    if (CAN.sendMsgBuf(id, 0, len, data) == CAN_OK)
    {
        return true;
    }

    return false;
}