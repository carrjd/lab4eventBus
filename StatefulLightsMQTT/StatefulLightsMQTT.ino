#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
//#include <Adafruit_INA219.h>
#include <string.h>


//const char* ssid = "UnimatrixZero";
//const char* password = "novi145$";
const char* ssid = "WobblyHouse";
const char* password = "5w0rd5gun5&danc1ng";
const char* mqtt_server = "192.168.1.240";
bool doorState = 1;  //door open = 1
WiFiClient espClient;
PubSubClient client(espClient);

int YEL = 16; // led connected to D0 (Yellow)
int RE = 15; // led connected to D8 (Red)
int GRE = 2; // led connected to D4 (Green)
void callback(char* topic, byte* payload, unsigned int length) {
 Serial.print("Message arrived [");
 Serial.print(topic);
 Serial.print("] ");
 char receivedChar;
 String receivedCh;
 for (int i=0;i<length;i++) {
  receivedCh += (char)payload[i];
  Serial.print(receivedCh);
  Serial.println();
}
String str(receivedChar);
Serial.println(str);

  goToState(receivedCh);
 
} 
void goToState(String color)
{
  Serial.println(color);
  if(doorState){
  if (color=="green"){
      digitalWrite(GRE, HIGH);
      digitalWrite(RE, LOW);
      digitalWrite(YEL, LOW);
    }
  if (color=="yellow"){
      digitalWrite(GRE, LOW);
      digitalWrite(RE, LOW);
      digitalWrite(YEL, HIGH);
  }
  if (color=="red"){
      digitalWrite(GRE, LOW);
      digitalWrite(RE, HIGH);
      digitalWrite(YEL, LOW);
  }
  if (color=="off"){
      digitalWrite(GRE, LOW);
      digitalWrite(RE, LOW);
      digitalWrite(YEL, LOW);
  }
  if (color=="all"){
      digitalWrite(GRE, HIGH);
      digitalWrite(RE, HIGH);
      digitalWrite(YEL, HIGH);
    }
  }
   if (color =="open"){
   doorState=1;
   } 
   if (color == "closed"){
    doorState=0;
      digitalWrite(GRE, LOW);
      digitalWrite(RE, LOW);
      digitalWrite(YEL, LOW);
    }
  
  }

void reconnect() {
 // Loop until we're reconnected
 while (!client.connected()) {
 Serial.print("Attempting MQTT connection...");
 // Attempt to connect
 if (client.connect("ESP8266 Client")) {
  Serial.println("connected");
  // ... and subscribe to topic
  client.subscribe("ledStatus");
  client.subscribe("doorStatus");
 } else {
  Serial.print("failed, rc=");
  Serial.print(client.state());
  Serial.println(" try again in 5 seconds");
  // Wait 5 seconds before retrying
  delay(5000);
  }
 }
}
 
void setup()
{
 Serial.begin(9600);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  pinMode(YEL, OUTPUT);
  pinMode(RE, OUTPUT);
  pinMode(GRE, OUTPUT);
  goToState("off");
 client.setServer(mqtt_server, 1883);
 client.setCallback(callback);
 
}
 
void loop()
{
 if (!client.connected()) {
  reconnect();
 }
 client.loop();
}
