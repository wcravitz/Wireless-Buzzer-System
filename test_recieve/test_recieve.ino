#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>

const int ledPin = 5;
const int buttonPin = 4;
bool locked = false;
int lastBuzzed;

RF24 radio(9, 10);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 00;   // Address of our node in Octal format ( 04,031, etc)

void setup() {
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  network.begin(90, this_node); //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  if (!locked) {
    network.update();
    while ( network.available() ) {// Is there any incoming data
      RF24NetworkHeader header;
      unsigned long incomingData;
      network.read(header, &incomingData, sizeof(incomingData)); // Read the incoming data
      if (incomingData) {
        lastBuzzed = header.from_node;
        Serial.println((String) lastBuzzed + " just buzzed.");
        locked = true;
        Serial.println("Turn the light on");
        digitalWrite(ledPin, 1);
        Serial.println("The light is on");
      }
    }
  } else {
    if (digitalRead(buttonPin)) {
      digitalWrite(ledPin, 0);
      locked = false;
    }
  }
}
