#ifndef OBD_H
#define OBD_H

#include <Arduino.h>

//------------------------------------
// Main OBD Function
//------------------------------------

void OBD_ProcessRequest(unsigned long id, byte len, byte *data);

//------------------------------------
// PID Handler Functions
//------------------------------------

void HandleCoolantTemperature();
void HandleEngineRPM();
void HandleVehicleSpeed();
void HandleFuelLevel();

#endif