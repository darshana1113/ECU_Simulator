

#include "can.h"
#include "obd.h"

//-------------------------------------
// List of Supported PIDs
//-------------------------------------

byte pidList[] =
{
    PID_COOLANT_TEMP,
    PID_ENGINE_RPM,
    PID_VEHICLE_SPEED,
    PID_FUEL_LEVEL
};

byte currentPID = 0;

//-------------------------------------

void setup()
{
    Serial.begin(115200);

    CAN_Init();

    Serial.println();
    Serial.println("=================================");
    Serial.println("      OBD-II Scanner Started");
    Serial.println("=================================");
}

//-------------------------------------

void loop()
{
    //---------------------------------
    // Send Request
    //---------------------------------

    OBD_SendRequest(pidList[currentPID]);

    //---------------------------------
    // Wait for ECU Response
    //---------------------------------

    unsigned long startTime = millis();

    while(millis() - startTime < 500)
    {
        unsigned long id;
        byte len;
        byte data[8];

        if(CAN_Receive(&id, &len, data))
        {
            OBD_ProcessResponse(id, len, data);
            break;
        }
    }

    //---------------------------------
    // Next PID
    //---------------------------------

    currentPID++;

    if(currentPID >= sizeof(pidList))
    {
        currentPID = 0;
    }

    delay(5000);
}