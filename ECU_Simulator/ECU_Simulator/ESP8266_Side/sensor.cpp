#include "sensor.h"         // responsible for providing sensor value when ECU needs 

DHT dht(DHTPIN, DHTTYPE);  // without sensor.h file wouldn't know that DHTPIN or DHTTYPE


//----------------------------------
// Initialize DHT22
//----------------------------------

bool Sensor_Init()     // initialization of DHT22
{
    dht.begin();    // it prepares the GPIO pin and communication timing required to communicate with the DHT22

    Serial.println("DHT22 Initialized");

    return true;
}


//----------------------------------
// DHT22 Temperature
//----------------------------------

float Sensor_ReadTemperature()
{
    return dht.readTemperature();
}


//----------------------------------
// DHT22 Humidity
//----------------------------------

float Sensor_ReadHumidity()
{
    return dht.readHumidity();
}


//----------------------------------
// Simulated RPM
//----------------------------------

int Sensor_ReadRPM()
{
    return 2500;
}


//----------------------------------
// Simulated Vehicle Speed
//----------------------------------

byte Sensor_ReadVehicleSpeed()
{
    return 65;
}


//----------------------------------
// Simulated Fuel Level
//----------------------------------

float Sensor_ReadFuelLevel()
{
    return 75.0;
}