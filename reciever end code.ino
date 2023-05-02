#include <SoftwareSerial.h>
#include <mcp2515.h>

SoftwareSerial bluetooth(0, 1); // RX, TX pins of HC-05 Bluetooth module

MCP2515 mcp2515(10);

struct can_frame HUM;
struct can_frame TEMP;
struct can_frame SMOKE;
struct can_frame LDR;
struct can_frame FIRE;

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
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
  if(mcp2515.readMessage(&HUM) == MCP2515::ERROR_OK) {
    String hum_data = "Humidity: " + String(HUM.data[0]) + "." + String(HUM.data[1]) + "%";
    bluetooth.println(hum_data);
  }

  if(mcp2515.readMessage(&TEMP) == MCP2515::ERROR_OK) {
    String temp_data = "Temperature: " + String(TEMP.data[0]) + "." + String(TEMP.data[1]) + "C";
    bluetooth.println(temp_data);
  }

  if(mcp2515.readMessage(&SMOKE) == MCP2515::ERROR_OK) {
    String smoke_data = "Smoke: " + String(SMOKE.data[0]) + "." + String(SMOKE.data[1]) + "." + String(SMOKE.data[2]);
    bluetooth.println(smoke_data);
  }

  if(mcp2515.readMessage(&LDR) == MCP2515::ERROR_OK) {
    String ldr_data = "Light: " + String(LDR.data[0]) + "." + String(LDR.data[1]) + "." + String(LDR.data[2]);
    bluetooth.println(ldr_data);
  }

  if(mcp2515.readMessage(&FIRE) == MCP2515::ERROR_OK) {
    String fire_data = "Fire: " + String(FIRE.data[0]);
    bluetooth.println(fire_data);
  }
}
