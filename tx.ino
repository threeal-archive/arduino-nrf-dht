#include "DHT.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

DHT DHT_sens(2, DHT11);

RF24 radio(9, 10);
const byte address[6] = "00001";

void setup() {
  DHT_sens.begin();

  Serial.begin(9600);

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  float h = DHT_sens.readHumidity();
  float t = DHT_sens.readTemperature();

  unsigned char data[2 + sizeof(h) + sizeof(t)];
  data[0] = 0xFF;
  data[1] = 0xFF;

  memcpy(data + 2, &h, sizeof(h));
  memcpy(data + 2 + sizeof(h), &t, sizeof(t));

  radio.write(data, sizeof(data));

  delay(10);
}