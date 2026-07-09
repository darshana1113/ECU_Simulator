#ifndef OBD_H
#define OBD_H

#include <Arduino.h>

//-------------------------------------
// Supported OBD-II PIDs
//-------------------------------------

#define PID_COOLANT_TEMP   0x05
#define PID_ENGINE_RPM     0x0C
#define PID_VEHICLE_SPEED  0x0D
#define PID_FUEL_LEVEL     0x2F

//-------------------------------------
// Scanner Functions
//-------------------------------------

void OBD_SendRequest(byte pid);

void OBD_ProcessResponse(unsigned long id, byte len, byte *data);

#endif