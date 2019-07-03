#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>

unsigned long buzzed;

RF24 radio(9, 10);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
//const uint16_t team_nodes[] = {011, 021, 031, 041, 051}; 
const uint16_t team_nodes[] = {012, 022, 032, 042, 052};
const uint16_t team = 02;
const uint16_t main = 00;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  network.begin(90, team); //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
}

void loop() {
  network.update();
  //===== Receiving =====//
  while (network.available()) { // Is there any incoming data
    RF24NetworkHeader header;
    unsigned long incomingData;
    network.read(header, &incomingData, sizeof(incomingData)); // Read the incoming data
    // 1 means buzzed (buzzer), 20 means turn buzzer led on (main), 30 means reset (main)
    Serial.print(incomingData);
    //===== Sending =====//
    if (incomingData == 1) {
      buzzed = header.from_node;
      RF24NetworkHeader header1(main);
      network.write(header1, &buzzed, sizeof(buzzed));
      Serial.println("written");
    } else {
      unsigned long buzzerState = 1;
      if (incomingData == 3) {
        buzzerState = 0;
      }
      Serial.println(buzzerState);
      RF24NetworkHeader header2(team_nodes[(buzzed-2)/8 - 1]); // buzzed will always be initialized before
      network.write(header2, &buzzerState, sizeof(buzzerState));
    } 
  }
}
