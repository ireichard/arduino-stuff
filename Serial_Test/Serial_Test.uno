#include "motherboard_packet_factory.h"

Motherboard_Packet_Factory motherboard_packet_factory;

void setup() {
  pinMode(11, OUTPUT);
  
  Serial.begin(115200);
  Serial.setTimeout(1);
  while(!Serial) {;}
}

void loop() {
  while (!Serial.available());
  char x = Serial.read();
  
  if (x == motherboard_packet_factory.HEADER_BYTE) {
    digitalWrite(11, LOW);
    char req_type = Serial.read();
    char x = Serial.read();
    if (x == motherboard_packet_factory.END_BYTE) {
      
      Packet p = motherboard_packet_factory.package_response(req_type);
      for (int i = 0; i < p.length; i++) {
        Serial.write(p.packet[i]);
      }
    }
  } else {
    digitalWrite(11, HIGH);
  }

  char header_code = 0xA4;
  char message = 0x03;
  char filler = 0x00;
  char end_code = 0xA0;
  
  Serial.write(0xA4);
  Serial.write(0x03);
  
  for (int i = 0; i < 4; i++) {
    Serial.write(0x00);
  }
  
  Serial.write(0xA0);
}
