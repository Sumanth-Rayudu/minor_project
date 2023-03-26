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

  HUM.can_id = 0x100;
  HUM.can_dlc = 2;
  TEMP.can_id = 0x101;
  TEMP.can_dlc = 2;
  SMOKE.can_id = 0x102;
  SMOKE.can_dlc = 3;
  LDR.can_id = 0x103;
  LDR.can_dlc = 3;
  FIRE.can_id = 0x104;
  FIRE.can_dlc = 1;
}

void loop() {
  int HUMD = dht.readHumidity();
  HUM.can_id = 0x100;
  HUM.data[0] = HUMD;
  mcp2515.sendMessage(&HUM);

  int TEM = dht.readTemperature();
  TEMP.can_id = 0x101;
  TEMP.data[0] = TEM;
  mcp2515.sendMessage(&TEMP);

  int SMO = analogRead(A0);
  int SMO_high = (SMO >> 8) & 0xFF;
  int SMO_low = SMO & 0xFF;
  SMOKE.can_id = 0x102;
  SMOKE.data[0] = SMO_high;
  SMOKE.data[1] = SMO_low;
  SMOKE.data[2] = FIRalert;
  mcp2515.sendMessage(&SMOKE);

  int LDR_val = analogRead(A1);
  int LDR_high = (LDR_val >> 8) & 0xFF;
  int LDR_low = LDR_val & 0xFF;
  LDR.can_id = 0x103;
  LDR.data[0] = LDR_high;
  LDR.data[1] = LDR_low;
  LDR.data[2] = FIRalert;
  mcp2515.sendMessage(&LDR);

  int FIR_val = analogRead(Fir);
  FIRE.can_id = 0x104;
  FIRE.data[0] = FIR_val;
  mcp2515.sendMessage(&FIRE);

  delay(1000);
}
