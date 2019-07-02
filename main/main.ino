#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>

const int buttonPin = 4;
bool locked = false;
unsigned long buzzTeam;
unsigned long buzzed;

RF24 radio(9, 10);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const int leds[2][2] = {{2,3},{5,6}};
const uint16_t teams[] = {01, 02};
const uint16_t main = 00;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  network.begin(90, main); //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
}

void loop() {
  network.update();
  //===== Receiving =====//
  if (!locked) {
    while (network.available()) { // Is there any incoming data
      RF24NetworkHeader header;
      network.read(header, &buzzed, sizeof(buzzed)); // Read the incoming data
      buzzTeam = header.from_node;
      digitalWrite(leds[buzzTeam-1][buzzed-1], HIGH);
      unsigned long ledOn = 2;
      RF24NetworkHeader header1(teams[buzzTeam-1]);
      network.write(header1, &ledOn, sizeof(ledOn));
      locked = true;
    }
  } else {
    if (digitalRead(buttonPin)) {
      digitalWrite(leds[buzzTeam-1][buzzed-1], LOW);
      unsigned long ledOff = 3;
      RF24NetworkHeader header2(teams[buzzTeam-1]);
      network.write(header2, &ledOff, sizeof(ledOff));
      locked = false;
    }
  }

}
