#include <SPI.h>
#include <mcp_can.h>
#include <DHT.h>

// ------------------- DHT22 -------------------
#define DHTPIN D2          // GPIO4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// ------------------- MCP2515 -------------------
#define CAN_CS D8          // GPIO15

MCP_CAN CAN(CAN_CS);

void setup()
{
  Serial.begin(115200);

  dht.begin();

  // Initialize CAN
  while (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK)
  {
    Serial.println("CAN Init Failed");
    delay(1000);
  }

  CAN.setMode(MCP_NORMAL);

  Serial.println("==================================");
  Serial.println(" ECU Simulator Started");
  Serial.println(" Waiting for OBD-II Request...");
  Serial.println("==================================");
}

void loop()
{
  unsigned long rxID;
  byte len;
  byte rxBuf[8];

  // Check if CAN message received
  if (CAN.checkReceive() == CAN_MSGAVAIL)
  {
    CAN.readMsgBuf(&rxID, &len, rxBuf);

    Serial.println("\n------------------------------");
    Serial.print("Received CAN ID : 0x");
    Serial.println(rxID, HEX);

    // Check if OBD-II Functional Request
    if (rxID == 0x7DF)
    {
      byte mode = rxBuf[1];
      byte pid  = rxBuf[2];

      Serial.print("Mode : 0x");
      Serial.println(mode, HEX);

      Serial.print("PID  : 0x");
      Serial.println(pid, HEX);

      // Only support Mode 01 PID 05
      if (mode == 0x01 && pid == 0x05)
      {
        Serial.println("Coolant Temperature Requested");

        // Read DHT22 Temperature
        float temp = dht.readTemperature();

        if (isnan(temp))
        {
          Serial.println("DHT22 Read Failed!");
          return;
        }

        Serial.print("DHT22 Temperature = ");
        Serial.print(temp);
        Serial.println(" °C");

        // OBD-II Formula
        // A = Temperature + 40
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

        CAN.sendMsgBuf(0x7E8, 0, 8, response);

        Serial.println("OBD-II Response Sent");

        Serial.print("Response Data : ");

        for (int i = 0; i < 8; i++)
        {
          Serial.print(response[i], HEX);
          Serial.print(" ");
        }

        Serial.println();
      }
    }
  }
}