
#ifndef CAN_H
#define CAN_H

#include <SPI.h>
#include <mcp_can.h>

#define CAN_CS 10

#define CAN_SPEED CAN_500KBPS
#define CAN_CLOCK MCP_8MHZ

extern MCP_CAN CAN;

bool CAN_Init();

bool CAN_Send(unsigned long id, byte len, byte *data);

bool CAN_Receive(unsigned long *id, byte *len, byte *data);

#endif