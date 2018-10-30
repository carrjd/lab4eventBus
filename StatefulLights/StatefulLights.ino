//Robo India tutorial On Controlling LED on WEB (modified by James Carr)
//https://www.roboindia.com/tutorials


#include <ESP8266WiFi.h>
 
//const char* ssid = "UnimatrixZero";
//const char* password = "novi145$";
const char* ssid = "WobblyHouse";
const char* password = "5w0rd5gun5&danc1ng";



int YEL = 16; // led connected to D0 (Yellow)
int RE = 15; // led connected to D8 (Red)
int GRE = 2; // led connected to D4 (Green)

bool YellowOn = 0;
bool RedOn = 0;
bool GreenOn = 0;

bool cycle = 0; //flag if the cycle function is on or off
WiFiServer server(80);
 
void setup() 
{
  Serial.begin(115200);
  pinMode(YEL, OUTPUT);
  digitalWrite(YEL, LOW);
  pinMode(RE, OUTPUT);
  digitalWrite(RE, LOW);
  pinMode(GRE, OUTPUT);
  digitalWrite(GRE, LOW);

  
  Serial.print("Connecting to Internet ");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
 
 /*-------- server started---------*/ 
  server.begin();
  Serial.println("Server started");
 
  /*------printing ip address--------*/
  Serial.print("IP Address of network: ");
  Serial.println(WiFi.localIP());
  Serial.print("Copy and paste the following URL: https://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}
 
void loop() 
  {
if (cycle){
      Serial.println("Cycling lights");
      digitalWrite(GRE, HIGH);
      delay(100);
      digitalWrite(GRE, LOW);
      digitalWrite(YEL, HIGH);
      delay(100);
      digitalWrite(YEL, LOW);
      digitalWrite(RE, HIGH);
      delay(100);
      digitalWrite(RE, LOW);        
    }

    
    WiFiClient client = server.available();
    client.setTimeout(1000);    
    if (!client) 
    {
      return;
    }
  Serial.println("Waiting for new client");   
  while(!client.available())
  {
    delay(1);
  }
 
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
 
  int value = LOW;
  if (request.indexOf("/green") != -1)  
  {
    
      digitalWrite(GRE, HIGH);
      digitalWrite(RE, LOW);
      digitalWrite(YEL, LOW);
      
  }
    
  if (request.indexOf("/yellow") != -1)  
  {
      digitalWrite(GRE, LOW);
      digitalWrite(RE, LOW);
      digitalWrite(YEL, HIGH);
      

  }
  
  if (request.indexOf("/red") != -1)  
  {
      digitalWrite(GRE, LOW);
      digitalWrite(RE, HIGH);
      digitalWrite(YEL, LOW);
  }
    if (request.indexOf("/off") != -1)  
  {
      digitalWrite(GRE, LOW);
      digitalWrite(RE, LOW);
      digitalWrite(YEL, LOW);
  }
    if (request.indexOf("/all") != -1)  
  {
      digitalWrite(GRE, HIGH);
      digitalWrite(RE, HIGH);
      digitalWrite(YEL, HIGH);
  }
  
  if (request.indexOf("/timerLoop/start") != -1)  
  {
    cycle = 1;
  }
    if (request.indexOf("/timerLoop/stop") != -1)  
  {
    cycle = 0;
  }


 Serial.println(cycle);
/*------------------Creating html page---------------------*/

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); 
  client.println("");
  client.println("");
 
  client.print("LED is: ");
 
  if(value == HIGH) 
  {
    client.print("ON");
  } 
  else 
  {
    client.print("OFF");
  }
  client.println("");
  client.println("ON");
  client.println("OFF");  
  client.println("");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
}
