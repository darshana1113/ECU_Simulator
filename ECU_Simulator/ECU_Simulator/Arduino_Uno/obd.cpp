#include "obd.h"
#include "can.h"

//=====================================================
// Send OBD-II Request
//=====================================================

void OBD_SendRequest(byte pid)
{
    byte request[8] =
    {
        0x02,       // Number of additional bytes
        0x01,       // Mode 01 (Show Current Data)
        pid,        // PID    //05 for coolent temp
        0x00,
        0x00,
        0x00,
        0x00,
        0x00
    };

    if(CAN_Send(0x7DF, 8, request))
    {
        Serial.println();
        Serial.println("==============================");
        Serial.println("OBD-II Request Sent");
        Serial.println("==============================");

        Serial.print("PID : 0x");
        Serial.println(pid, HEX);
    }
    else
    {
        Serial.println("CAN Send Failed");
    }
}

//=====================================================
// Process ECU Response
//=====================================================

void OBD_ProcessResponse(unsigned long id, byte len, byte *data)
{
    // Accept only Engine ECU response
    if(id != 0x7E8)
        return;

    if(len < 4)
    {
        Serial.println("Invalid ECU Response");
        return;
    }

    byte mode = data[1];
    byte pid  = data[2];

    if(mode != 0x41)    // offset + 01 mode (current value+-)
    {
        Serial.println("Invalid Response Mode");
        return;
    }

    Serial.println();
    Serial.println("==============================");
    Serial.println(" ECU Response");
    Serial.println("==============================");

    Serial.print("PID : 0x");
    Serial.println(pid, HEX);

    switch(pid)
    {
        //------------------------------------------------
        // Coolant Temperature
        //------------------------------------------------

        case PID_COOLANT_TEMP:
        {
            float temp = data[3] - 40;

            Serial.print("Coolant Temperature : ");
            Serial.print(temp);
            Serial.println(" °C");

            break;
        }

        //------------------------------------------------
        // Engine RPM
        //------------------------------------------------

        case PID_ENGINE_RPM:
        {
            unsigned int value = (data[3] << 8) | data[4];

            float rpm = value / 4.0;

            Serial.print("Engine RPM : ");
            Serial.println(rpm);

            break;
        }

        //------------------------------------------------
        // Vehicle Speed
        //------------------------------------------------

        case PID_VEHICLE_SPEED:
        {
            byte speed = data[3];

            Serial.print("Vehicle Speed : ");
            Serial.print(speed);
            Serial.println(" km/h");

            break;
        }

        //------------------------------------------------
        // Fuel Level
        //------------------------------------------------

        case PID_FUEL_LEVEL:
        {
            float fuel = (data[3] * 100.0) / 255.0;

            Serial.print("Fuel Level : ");
            Serial.print(fuel);
            Serial.println(" %");

            break;
        }

        default:
        {
            Serial.println("Unsupported PID");
            break;
        }
    }

    Serial.println("==============================");
}