#ifndef CAN_H         // If CAN_H has not been defined, compile this file.
#define CAN_H         

#include <SPI.h>       // esp8266 can not directly talk with mcp2515 so for communication we need to use SPI protoco
#include <mcp_can.h>

// MCP2515 CS Pin
#define CAN_CS D8

// CAN Speed
#define CAN_SPEED CAN_500KBPS

// Crystal Frequency
#define CAN_CLOCK MCP_8MHZ

// Global CAN Object
extern MCP_CAN CAN;         // all files need to access to the same CAN object that why it have a global object
                          // Using extern allows other files to access the same object without creating duplicate instances

// Initialize CAN
bool CAN_Init();

// Receive CAN Frame
bool CAN_Receive(unsigned long *id, byte *len, byte *data);

// Send CAN Frame
bool CAN_Send(unsigned long id, byte len, byte *data);

#endif