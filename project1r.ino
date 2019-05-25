#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(D4,D8); // CE, CSN
const byte address[6] = "VENKY";
void setup() {
  pinMode(D1 ,OUTPUT);
  pinMode(D2 ,OUTPUT);
  digitalWrite(D1 ,LOW);
  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(0, address);//we are ging 0 as address
  radio.setPALevel(RF24_PA_MIN);// giving minimum frequency
  radio.startListening();
}
void loop() {
  if (radio.available()) {
    char text[32] = "";//initially taking it as empty
    radio.read(&text, sizeof(text));
    Serial.println(text);
  digitalWrite(D1 ,HIGH);
  digitalWrite(D2 ,HIGH);
}
else
{
  digitalWrite(D1 ,LOW);
  digitalWrite(D2 ,LOW);
}
    delay(1100);
}
