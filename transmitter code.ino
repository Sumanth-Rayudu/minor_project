#include <SPI.h>
#include <mcp2515.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11

struct can_frame HUM;
struct can_frame TEMP;
struct can_frame SMOKE;
struct can_frame LDR;
struct can_frame FIRE;

const int gaspin = A0;
const int LDR_pin = A5;
const int IR_digi = 7;



MCP2515 mcp2515(10);
DHT dht(DHTPIN, DHTTYPE);

void setup() {

  pinMode(gaspin,INPUT);
  pinMode(IR_digi,INPUT);
  Serial.begin(9600);
  SPI.begin();
  dht.begin();
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS,MCP_8MHZ);
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
  HUM.can_id = 0xaa;
  HUM.data[0] = HUMD;
  mcp2515.sendMessage(&HUM);
  Serial.print("humidity : ");
  Serial.println(HUMD);
  delay(300);
    
  int TEM = dht.readTemperature();
  TEMP.can_id = 0xbb;
  TEMP.data[0] = TEM;
  mcp2515.sendMessage(&TEMP);
  Serial.print("Temperature : ");
  Serial.println(TEM);
   delay(300);

  
  int smokeVal = analogRead(gaspin);
  SMOKE.can_id = 0xcc;
  if(smokeVal > 40){
    Serial.println(smokeVal);

  }
  SMOKE.data[0] = smokeVal & 0xFF;
  SMOKE.data[1] = (smokeVal >> 8) & 0xFF;
  mcp2515.sendMessage(&SMOKE);
  Serial.print("smoke : ");
  Serial.println(smokeVal);
   delay(300);

  
  int ldrVal = analogRead(LDR_pin);
  LDR.can_id = 0xdd;
  LDR.data[0] = ldrVal & 0xFF;
  LDR.data[1] = (ldrVal >> 8) & 0xFF;
  mcp2515.sendMessage(&LDR);
  Serial.print("light intensity : ");
  Serial.println(ldrVal);
  delay(300);
  
  int irVal = digitalRead(IR_digi);
  FIRE.can_id = 0xee;
  if(irVal==0){
    FIRE.data[0] = 1;
    Serial.println("Fire Detected");
  }
  else {
    FIRE.data[0] = 0;
    Serial.println("No Fire Detected");
  }
  mcp2515.sendMessage(&FIRE);
  delay(300);
  
  delay(1000); // wait a second between readings
}
