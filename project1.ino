#include <ESP8266WiFi.h>
#include <PubSubClient.h>
const char* ssid = "SB-IOT1";
const char* password = "sb@iot11";
String command;
 
#define ORG "nrdaz1"
#define DEVICE_TYPE "nodemcu"
#define DEVICE_ID "1000"
#define TOKEN "2210416109"
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/data/fmt/json";
char topic1[] = "iot-2/cmd/home/fmt/String";
char authMethod[] = "use-token-auth";
char authMethod1[] = "use-token-auth";
char token[] = TOKEN;
char token1[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h> 
void callback(char* topic1, byte* payload, unsigned int payloadLength);
WiFiClient wifiClient;
//PubSubClient client(server, 1883,wifiClient);
PubSubClient client(server, 1883, callback, wifiClient);

RF24 radio(D4, D8); // CE, CSN

const byte address[6] = "VENKY";//address is a array and give the address name as numbers or string

void setup() {
  pinMode(D2,OUTPUT);
  radio.begin();//radio is a instance  
  radio.openWritingPipe(address);// giving address to the nRF
  radio.setPALevel(RF24_PA_MIN);//setting amplification level minimum
  radio.stopListening();//
  Serial.begin(9600);
 Serial.println();
 Serial.print("Connecting to ");
 Serial.print(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 } 
 Serial.println("");
 
 Serial.print("WiFi connected, IP address: ");
 Serial.println(WiFi.localIP());
   wifiConnect();
  mqttConnect();
}

void loop() {
  int ldr = analogRead(A0);
  if (isnan(ldr))
{
Serial.println("Failed to read from DHT sensor!");
delay(1000);
return;
}
PublishData(ldr);
 if (!client.loop()) {
    mqttConnect();
  }
delay(100);
if(ldr <= 500)
{
  digitalWrite(D2 ,LOW);
//  const char text[] = "lightoff";//transmiting hello world as data we can change it as our message 
  //radio.write(&text, sizeof(text));//it will transmit the data and size of it
 // Serial.print("lightoff");
  delay(1000);
}
else
{
  analogWrite(D2 ,ldr);
  const char text[] = "on";//transmiting hello world as data we can change it as our message 
  radio.write(&text, sizeof(text));//it will transmit the data and size of it
 // Serial.print("lighton");
  delay(1000);
}
}
void wifiConnect() {
  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("nWiFi connected, IP address: "); Serial.println(WiFi.localIP());
}
void mqttConnect() {
  if (!client.connected()) {
    Serial.print("Reconnecting MQTT client to "); Serial.println(server);
    while (!client.connect(clientId, authMethod, token)) {
      Serial.print(".");
      delay(500);
    }
    initManagedDevice();
    initManagedDevice1();
    Serial.println();
  }
}
void initManagedDevice() {
  if (client.subscribe(topic)) {
    Serial.println("subscribe to cmd OK");
  } else {
    Serial.println("subscribe to cmd FAILED");
  }
}
void initManagedDevice1() {
  if (client.subscribe(topic1)) {
    Serial.println("subscribe to cmd OK");
  } else {
    Serial.println("subscribe to cmd FAILED");
  }
}

void callback(char* topic1, byte* payload, unsigned int payloadLength) {
  Serial.print("callback invoked for topic: "); Serial.println(topic1);

  for (int i = 0; i < payloadLength; i++) {
   // Serial.println((char)payload[i]);
    command += (char)payload[i];
  }
Serial.println(command);
if(command == "lightoff"){
  digitalWrite(D2,LOW);
  Serial.println("Light is Switched Off");
}
else if(command == "lighton"){
  digitalWrite(D2,HIGH);
  Serial.println("Light is Switched On");
}
command ="";
}

void PublishData(float ldr){
  if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
 }
 String payload = "{\"d\":{\"ldr\":";
 payload += ldr;
  payload += "}}";
  Serial.print("Sending payload: ");
 Serial.println(payload); 
 if (client.publish(topic, (char*) payload.c_str())) {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }
}
