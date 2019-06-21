#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>

const int buttonPin = 5;

RF24 radio(9, 10);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 02;   // Address of this node in Octal format ( 04,031, etc)
const uint16_t master = 00;
      
void setup() {
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  pinMode(buttonPin, INPUT);
}

void loop() {
  network.update();
  unsigned long buttonValue = digitalRead(buttonPin);  // Read the button value
  RF24NetworkHeader header(master);     // (Address where the data is going)
  bool ok = network.write(header, &buttonValue, sizeof(buttonValue)); // Send the data
}
