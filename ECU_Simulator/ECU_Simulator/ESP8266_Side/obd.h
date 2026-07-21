// it contains the declaration of all OBD-related functions.

#ifndef OBD_H
#define OBD_H

#include <Arduino.h>    // Without including Arduino.h, the compiler would not recognize the byte data type used in your function declarations.

//------------------------------------
// Main OBD Function
//------------------------------------

void OBD_ProcessRequest(unsigned long id, byte len, byte *data);

//------------------------------------
// PID Handler Functions
//------------------------------------

// all PID handler functions have its own PID id 

void HandleCoolantTemperature();     // PID 05
void HandleEngineRPM();              //PID 0C
void HandleVehicleSpeed();          //PID 0D
void HandleFuelLevel();             //PID 2F

#endif