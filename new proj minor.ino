#include <SPI.h>
#include <mcp2515.h>
#include <DHT.h>

#define DHTPIN A3
#define DHTTYPE DHT11

struct can_frame HUM;
struct can_frame TEMP;
struct can_frame SMOKE;
struct can_frame LDR;
struct can_frame FIRE;

int Fir = A2;
int FIRalert = 1;

MCP2515 mcp2515(10);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  SPI.begin();
  dht.begin();
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS,MCP_8MHZ);
  mcp2515.setNormalMode();

  HUM.can_id = 0xaa;
  HUM.can_dlc = 2;
  TEMP.can_id = 0xbb;
  TEMP.can_dlc = 2;
  SMOKE.can_id = 0xcc;
  SMOKE.can_dlc = 3;
  LDR.can_id = 0xdd;
  LDR.can_dlc = 3;
  FIRE.can_id = 0xee;
  FIRE.can_dlc = 1;
}

void loop() {
  int HUMD = dht.readHumidity();
  HUM.data[0] = HUMD;
  mcp2515.sendMessage(&HUM);

  int TEMP = dht.readTemperature(); // Fix: Changed variable name to TEMP
  TEMP.data[0] = TEMP; // Fix: Changed array index to 0
  mcp2515.sendMessage(&TEMP);
  
  int SMO = analogRead(A1);
  SMOKE.data[0] = SMO >> 8; // Send the high byte
  SMOKE.data[1] = SMO & 0xFF; // Send the low byte
  mcp2515.sendMessage(&SMOKE);

  int LDRv = analogRead(A0);
  LDR.data[0] = LDRv >> 8; // Send the high byte
  LDR.data[1] = LDRv & 0xFF; // Send the low byte
  mcp2515.sendMessage(&LDR);

  int FIRv = analogRead(Fir);
  if (FIRv > FIRalert) {
    FIRE.data[0] = 0x01; // Send 0x01 if fire is detected
  } else {
    FIRE.data[0] = 0x00; // Send 0x00 if no fire is detected
  }
  mcp2515.sendMessage(&FIRE);
}
