#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>

#define buttonPin 5
#define ledPin 6

const uint16_t teamNodes[2][5] = {{011, 021, 031, 041, 051}, {012, 022, 032, 042, 052}};
const uint16_t teams[2] = {01, 02};
const int index = 1;
const int team = teams[index/5];
const int node = teamNodes[index/5][index%5];

RF24 radio(9, 10); // nRF24L01 (CE,CSN)
RF24Network network(radio); // Include the radio in the network
      
void setup() {
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  network.begin(90, node);  // (channel, node address)
  radio.setDataRate(RF24_2MBPS);
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  Serial.println(team);
  Serial.println(node);
}

void loop() {
  network.update();
  //===== Receiving =====//
  while (network.available()) { // Check if there is incoming data
    RF24NetworkHeader header;
    unsigned long ledState;
    network.read(header, &ledState, sizeof(ledState)); // Read the incoming data
    digitalWrite(ledPin, ledState);
  }
  //===== Sending =====//
  unsigned long buttonState = digitalRead(buttonPin); // Read the button value
  if (buttonState) {
    RF24NetworkHeader header1(team); // (Address where the data is going)
    Serial.println(network.write(header1, &buttonState, sizeof(buttonState))); // Send the data
  }
}
