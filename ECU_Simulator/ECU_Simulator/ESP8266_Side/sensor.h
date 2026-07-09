#ifndef SENSOR_H               // this is a header guard 
#define SENSOR_H

#include <DHT.h>

//----------------------------------
// DHT22 Configuration
//----------------------------------

#define DHTPIN D2
#define DHTTYPE DHT22

//----------------------------------
// Function Prototypes
//----------------------------------

bool Sensor_Init();   // declaration

float Sensor_ReadTemperature();
float Sensor_ReadHumidity();

// Simulated Automotive Sensors

int Sensor_ReadRPM();

byte Sensor_ReadVehicleSpeed();

float Sensor_ReadFuelLevel();

#endif