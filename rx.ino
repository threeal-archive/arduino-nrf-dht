#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);

  pinMode(8, INPUT);

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  float h, t;
  unsigned char data[2 + sizeof(h) + sizeof(t)];

  int read = sizeof(data);
  while (read > 0) {
    if (radio.available()) {
      radio.read(data + (sizeof(data) - read), read);
      for (read = 0; read < sizeof(data); ++read) {
        if (read < sizeof(data) - 1) {
          if (data[read] == 0xFF && data[read + 1] == 0xFF)
            break;
        }
      }

      for (int i = 0; i < sizeof(data) - read; ++i) {
        data[i] = data[i + read];
      }
    }
  }

  memcpy(&h, data + 2, sizeof(h));
  memcpy(&t, data + 2 + sizeof(h), sizeof(t));

  Serial.print("h=");
  Serial.print(h);
  Serial.print(", ");
  Serial.print("t=");
  Serial.println(t);
}
