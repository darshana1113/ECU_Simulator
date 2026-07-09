#include <SPI.h>
#include <mcp_can.h>

// MCP2515 CS Pin
#define CAN_CS_PIN 10

MCP_CAN CAN(CAN_CS_PIN);

void setup()
{
  Serial.begin(115200);

  // Initialize MCP2515
  while (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK)
  {
    Serial.println("CAN Init Failed...");
    delay(1000);
  }

  CAN.setMode(MCP_NORMAL);

  Serial.println("======================================");
  Serial.println("   OBD-II Scanner Started");
  Serial.println("======================================");
}

void loop()
{
  // -------------------------------
  // OBD-II Request Frame
  // Mode 01 -> Show Current Data
  // PID 05  -> Engine Coolant Temperature
  // -------------------------------

  byte request[8] =
  {
    0x02,
    0x01,
    0x05,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00
  };

  // Send Request
  CAN.sendMsgBuf(0x7DF, 0, 8, request);

  Serial.println("\n--------------------------------------");
  Serial.println("OBD-II Request Sent");
  Serial.println("CAN ID : 0x7DF");
  Serial.println("Request : 02 01 05");

  unsigned long startTime = millis();

  // Wait up to 1 second for ECU response
  while (millis() - startTime < 1000)
  {
    if (CAN.checkReceive() == CAN_MSGAVAIL)
    {
      unsigned long rxID;
      byte len;
      byte rxBuf[8];

      CAN.readMsgBuf(&rxID, &len, rxBuf);

      Serial.print("Received CAN ID : 0x");
      Serial.println(rxID, HEX);

      // Accept only Engine ECU Response
      if (rxID == 0x7E8)
      {
        Serial.println("ECU Response Received");

        Serial.print("Raw Data : ");

        for (int i = 0; i < len; i++)
        {
          if (rxBuf[i] < 0x10)
            Serial.print("0");

          Serial.print(rxBuf[i], HEX);
          Serial.print(" ");
        }

        Serial.println();

        //-------------------------------------------------
        // Decode OBD-II Response
        //-------------------------------------------------

        byte mode = rxBuf[1];
        byte pid = rxBuf[2];
        byte A = rxBuf[3];

        if (mode == 0x41 && pid == 0x05)
        {
          float coolantTemp = A - 40;

          Serial.println();
          Serial.println("===== OBD-II Decoded Data =====");

          Serial.print("Mode                : 0x");
          Serial.println(mode, HEX);

          Serial.print("PID                 : 0x");
          Serial.println(pid, HEX);

          Serial.print("Coolant Temperature : ");
          Serial.print(coolantTemp);
          Serial.println(" °C");

          Serial.println("===============================");
        }
        else
        {
          Serial.println("Unknown OBD-II Response");
        }

        break;
      }
    }
  }

  delay(5000);
}