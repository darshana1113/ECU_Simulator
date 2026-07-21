// implements the OBD-2 protocol. CAN req is received this module checks whether it is a valid OBD-2 request ,
// identifies the requested mode and pid , and create a response frame and send it back over the can bus 

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

    byte mode = data[1];// current data     // Byte	Value	Meaning
    byte pid  = data[2];                  //   0	     02	     Number of useful bytes
                                         //    1	     01	     OBD Mode
                                        //    2	     05 	 PID(coolant Temp)
    

    Serial.println();
    Serial.println("========== OBD Request ==========");

    Serial.print("Mode : 0x");
    Serial.println(mode, HEX);

    Serial.print("PID  : 0x");      // print pid id of coolant temp , RPM , fuel 
    Serial.println(pid, HEX);

    //--------------------------------
    // Only support Mode 01
    //--------------------------------

    if(mode != 0x01)          // 01 mode means show current data
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
                0x03,    // three useful bytes : 41 , 05 , A
                0x41,    // 40( offset by OBD2) + 01(current data) = 41
                0x05,    // coolant temp
                A,       // A = Temp + 40 
                0x00,
                0x00,
                0x00,
                0x00     // Creates an array of 8 bytes
            };

            CAN_Send(0x7E8,8,response);         // 0x7DF : request from scanner 
                                               // 0x7E8 : response from ECU

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