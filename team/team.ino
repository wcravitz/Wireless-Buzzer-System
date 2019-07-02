#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>

bool locked = false;
unsigned long buzzed;

RF24 radio(9, 10);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 01;   // Address of our node in Octal format ( 04,031, etc)
const uint16_t team_nodes[] = {011, 021, 031, 041, 051}; 
const uint16_t main = 00;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  network.begin(90, this_node); //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
}

void loop() {
  network.update();
  while ( network.available() ) {// Is there any incoming data
    RF24NetworkHeader header;
    unsigned long incomingData;
    network.read(header, &incomingData, sizeof(incomingData)); // Read the incoming data
    if (incomingData) {buzzed = header.from_node;}
    RF24NetworkHeader header1(main);
    bool ok = network.write(header1, &incomingData, sizeof(incomingData));
    if (incomingData == 2) {
      RF24NetworkHeader header2(team_nodes[buzzed]);
      unsigned long sendTo = 1;
      bool ok = network.write(header1, &sendTo, sizeof(sendTo));
    }
  }
}
