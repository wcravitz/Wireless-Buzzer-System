#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>

bool locked = false;
unsigned long buzzed;
const uint16_t team_nodes[2][5] = {{011, 021, 031, 041, 051}, {012, 022, 032, 042, 052}};
const uint16_t teams[2] = {01, 02};
const uint16_t main = 00;
const int index = 0;
const int team = teams[index];

RF24 radio(9, 10); // nRF24L01 (CE,CSN)
RF24Network network(radio); // Include the radio in the network

void setup() {
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  network.begin(90, team); // (channel, node address)
  radio.setDataRate(RF24_2MBPS);
}

void loop() {
  network.update();
  //===== Receiving =====//
  while (network.available()) { // Is there any incoming data
    RF24NetworkHeader header;
    unsigned long incomingData;
    network.read(header, &incomingData, sizeof(incomingData)); // Read the incoming data
    // 1 means buzzed (buzzer), 2 means turn buzzer led on (main), 3 means reset (main)
    Serial.print(incomingData);
    
    //===== Sending =====//
    if (!locked) { // If the system has been reset
      if (incomingData == 1) {
        buzzed = ((header.from_node-team)/8 - 1);
        RF24NetworkHeader header1(main);
        network.write(header1, &buzzed, sizeof(buzzed));
        locked = true;
      }
    } else {
      if (incomingData == 2 || incomingData == 3) {
        unsigned long buzzerState = 1;
        if (incomingData == 3) { // Reset the system
          buzzerState = 0;
          locked = false;
        }
        Serial.print(incomingData);
        Serial.println(buzzed);
        RF24NetworkHeader header2(team_nodes[index][buzzed]); // buzzed will always be initialized before
        network.write(header2, &buzzerState, sizeof(buzzerState));
      } 
    }
  }
}
