// Get ESP8266 going with Arduino IDE
// - https://github.com/esp8266/Arduino#installing-with-boards-manager
// Required libraries (sketch -> include library -> manage libraries)
// - PubSubClient by Nick ‘O Leary
// - DHT sensor library by Adafruit

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <NewPing.h>

const char* ssid = "WobblyHouse";
const char* password = "5w0rd5gun5&danc1ng";
const char* mqtt_server = "192.168.1.240";
const int trigPin = 4;
const int echoPin = 5;
const int maxDist = 200;
long duration;
int distance;

WiFiClient espClient;
PubSubClient client(espClient);

NewPing sonar(trigPin, echoPin, maxDist); // NewPing setup of pins and maximum distance.
void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

long lastMsg = 0;

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 200) {
    lastMsg = now;
        Serial.print("Ping: ");
  distance = sonar.ping_cm();
  Serial.print(distance); // Send ping, get distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");
  
  if (distance >=30 || distance ==0){
    Serial.print("green");
    client.publish("ledStatus","green");
    //lightOnTest();
    }
  if (distance < 30 && distance >=17){
    Serial.print("yellow");
    client.publish("ledStatus","yellow");
    }
  if (distance < 17 && distance >=5){
    Serial.print("red");
    client.publish("ledStatus","red");
    } 
  if (distance < 7 && distance > 0){
    Serial.print("all");
    client.publish("ledStatus","all");
    }
    
  }
}
