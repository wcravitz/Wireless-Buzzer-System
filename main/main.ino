#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>

#define buttonPin 4

bool locked = false;
unsigned long buzzTeam;
unsigned long buzzed;
unsigned long ledState;
const int leds[2][2] = {{5,6},{2,3}};
const uint16_t teams[2] = {01, 02};
const uint16_t main = 00;

RF24 radio(9, 10); // nRF24L01 (CE,CSN)
RF24Network network(radio); // Include the radio in the network

void setup() {
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  network.begin(90, main); //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  pinMode(buttonPin, INPUT); 
  for (int i = 0; i < 2; i++) { // Initialize all leds as inputs
    for (int j = 0; j < 2; j++) {
      pinMode(leds[i][j], OUTPUT);
    }
  }
}

void loop() {
  //===== Receiving =====//
  if (!locked) {
    network.update();
    while (network.available()) { // Is there any incoming data
      RF24NetworkHeader header;
      network.read(header, &buzzed, sizeof(buzzed)); // Read the incoming data
      buzzTeam = header.from_node - 1;
      digitalWrite(leds[buzzTeam][buzzed], HIGH);
      ledState = 2;
      safeTransmit(20, ledState, teams[buzzTeam]);
      locked = true;
    }
  } else {
    if (digitalRead(buttonPin)) {
      digitalWrite(leds[buzzTeam][buzzed], LOW);
      ledState = 3;
      locked = false;
      network.update();
      safeTransmit(20, ledState, teams[buzzTeam]);
    }
    
  }
}

void safeTransmit(int num, unsigned long message, const uint16_t dest) {
  for (int i = 0; i < num; i++) {
    RF24NetworkHeader header2(dest);
    network.write(header2, &message, sizeof(message));
    network.update();
  }
}
