#include "obd.h"
#include "can.h"
#include "sensor.h"

//------------------------------------
// Process Incoming OBD-II Request
//------------------------------------
void OBD_ProcessRequest(unsigned long id, byte len, byte *data)
{
    // Only accept functional request
    if(id != 0x7DF)
        return;

    byte mode = data[1];
    byte pid  = data[2];

    Serial.println();
    Serial.println("========== OBD Request ==========");

    Serial.print("Mode : 0x");
    Serial.println(mode, HEX);

    Serial.print("PID  : 0x");
    Serial.println(pid, HEX);

    //--------------------------------
    // Only support Mode 01
    //--------------------------------

    if(mode != 0x01)
    {
        Serial.println("Unsupported Mode");
        return;
    }

    //--------------------------------
    // PID Selection
    //--------------------------------

    switch(pid)
    {
        //-------------------------------------------------
        // PID 05 : Coolant Temperature
        //-------------------------------------------------

        case 0x05:
        {
            float temp = Sensor_ReadTemperature();

            if(isnan(temp))
            {
                Serial.println("DHT22 Read Failed");
                return;
            }

            byte A = (byte)(temp + 40);

            byte response[8] =
            {
                0x03,
                0x41,
                0x05,
                A,
                0x00,
                0x00,
                0x00,
                0x00
            };

            CAN_Send(0x7E8,8,response);

            Serial.print("Coolant Temperature : ");
            Serial.print(temp);
            Serial.println(" C");

            Serial.println("Response Sent");

            break;
        }

        //-------------------------------------------------
        // PID 0C : Engine RPM
        //-------------------------------------------------

        case 0x0C:
        {
            int rpm = Sensor_ReadRPM();

            unsigned int value = rpm * 4;

            byte A = highByte(value);
            byte B = lowByte(value);

            byte response[8] =
            {
                0x04,
                0x41,
                0x0C,
                A,
                B,
                0x00,
                0x00,
                0x00
            };

            CAN_Send(0x7E8,8,response);

            Serial.print("Engine RPM : ");
            Serial.println(rpm);

            Serial.println("Response Sent");

            break;
        }

        //-------------------------------------------------
        // PID 0D : Vehicle Speed
        //-------------------------------------------------

        case 0x0D:
        {
            byte speed = Sensor_ReadVehicleSpeed();

            byte response[8] =
            {
                0x03,
                0x41,
                0x0D,
                speed,
                0x00,
                0x00,
                0x00,
                0x00
            };

            CAN_Send(0x7E8,8,response);

            Serial.print("Vehicle Speed : ");
            Serial.print(speed);
            Serial.println(" km/h");

            Serial.println("Response Sent");

            break;
        }

        //-------------------------------------------------
        // PID 2F : Fuel Level
        //-------------------------------------------------

        case 0x2F:
        {
            float fuel = Sensor_ReadFuelLevel();

            byte A = (byte)((fuel * 255.0) / 100.0);

            byte response[8] =
            {
                0x03,
                0x41,
                0x2F,
                A,
                0x00,
                0x00,
                0x00,
                0x00
            };

            CAN_Send(0x7E8,8,response);

            Serial.print("Fuel Level : ");
            Serial.print(fuel);
            Serial.println(" %");

            Serial.println("Response Sent");

            break;
        }

        //-------------------------------------------------
        // Unsupported PID
        //-------------------------------------------------

        default:
        {
            Serial.println("PID Not Supported");
            break;
        }
    }

    Serial.println("===============================");
}