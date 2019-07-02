#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>

const int ledPin = 2;
const int buttonPin = 3;

RF24 radio(9, 10);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t team_nodes[] = {011, 021, 031, 041, 051}; 
const int index = 1;
const uint16_t team1 = 01;
      
void setup() {
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  network.begin(90, team_nodes[index]);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  network.update();
  //===== Receiving =====//
  while ( network.available() ) {     // Is there any incoming data?
    RF24NetworkHeader header;
    unsigned long ledState;
    network.read(header, &ledState, sizeof(ledState)); // Read the incoming data
    if (ledState == index) {
      digitalWrite(ledPin, HIGH);
    } else {
      digitalWrite(ledPin, LOW);
    }
  }
  //===== Sending =====//
  unsigned long buttonState = digitalRead(buttonPin);  // Read the button value
  RF24NetworkHeader header1(team1);     // (Address where the data is going)
  bool ok = network.write(header1, &buttonState, sizeof(buttonState)); // Send the data
}
