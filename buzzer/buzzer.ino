#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>

//const int ledPin = 2;
const int buttonPin = 3;

RF24 radio(9, 10);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
//const uint16_t team_nodes[] = {011, 021, 031, 041, 051}; 
const uint16_t team_nodes[] = {012, 022, 032, 042, 052};
const uint16_t team = 01;
const int index = 2;

      
void setup() {
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  network.begin(90, team_nodes[index-1]);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  pinMode(buttonPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  network.update();
  //===== Receiving =====//
  while (network.available()) {     // Is there any incoming data?
    RF24NetworkHeader header;
    unsigned long ledState;
    network.read(header, &ledState, sizeof(ledState)); // Read the incoming data
    if (ledState == 1) {
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
  //===== Sending =====//
  unsigned long buttonState = digitalRead(buttonPin);  // Read the button value
  if (buttonState) {
    RF24NetworkHeader header1(team);     // (Address where the data is going)
    network.write(header1, &buttonState, sizeof(buttonState)); // Send the data
  }
}
