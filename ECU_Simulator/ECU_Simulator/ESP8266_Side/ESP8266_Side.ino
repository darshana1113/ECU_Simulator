#include "can.h"
#include "sensor.h"
#include "obd.h"

void setup()
{
    Serial.begin(115200);

    CAN_Init();

    Sensor_Init();

    Serial.println();
    Serial.println("==================================");
    Serial.println(" ECU Simulator Started");
    Serial.println("==================================");
}

void loop()
{
    unsigned long id;
    byte len;
    byte data[8];

    if(CAN_Receive(&id,&len,data))
    {
        OBD_ProcessRequest(id,len,data);
    }
}