/******************************************************************************
Reed_Switch_Example.ino
Example sketch for SparkFun's Reed Switch
  (https://www.sparkfun.com/products/8642)
Jim Lindblom @ SparkFun Electronics
May 3, 2016

The reed switch is a two-terminal, magnetically-actuated, normally-open switch.
Connect one end of the switch to ground, and the other to Arduino's D2 pin.

The D2 pin's internal pull-up resistor is used to bias the pin high. When the
switch closes, the pin should go low.

Development environment specifics:
Arduino 1.6.7
******************************************************************************/
const int REED_PIN = 12; // Pin connected to reed switch
//const int LED_PIN = 13; // LED pin - active-high

const int pinSwitch = 12; //Pin Reed

const int pinLed = 9; //Pin LED

int StatoSwitch = 0;



// Get ESP8266 going with Arduino IDE
// - https://github.com/esp8266/Arduino#installing-with-boards-manager
// Required libraries (sketch -> include library -> manage libraries)
// - PubSubClient by Nick ‘O Leary
// - DHT sensor library by Adafruit

#include <ESP8266WiFi.h>
#include <PubSubClient.h>


const char* ssid = "WobblyHouse";
const char* password = "5w0rd5gun5&danc1ng";
const char* mqtt_server = "192.168.1.240";


WiFiClient espClient;
PubSubClient client(espClient);


void setup() {
  Serial.begin(115200);
  pinMode(REED_PIN, INPUT_PULLUP);
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
  int proximity = digitalRead(REED_PIN); // Read the state of the switch
  if (proximity == LOW) // If the pin reads low, the switch is closed.
  {
    Serial.println("Switch closed");
    client.publish("doorStatus","closed");
 //   digitalWrite(LED_PIN, HIGH); // Turn the LED on
  }
  else
  {
    client.publish("doorStatus","open");
 //   digitalWrite(LED_PIN, LOW); // Turn the LED off
  }
 
    
  }
}


/*
void setup()

{

//pinMode(pinLed, OUTPUT); //Imposto i PIN

pinMode(pinSwitch, INPUT);
Serial.begin(9600);
}

void loop()

{

StatoSwitch = digitalRead(pinSwitch); //Leggo il valore del Reed

if (StatoSwitch == HIGH)

{
Serial.println("HIGH");
//digitalWrite(pinLed, HIGH);

}

else

{
Serial.println("LOW");
//digitalWrite(pinLed, LOW);

}

}
*/
