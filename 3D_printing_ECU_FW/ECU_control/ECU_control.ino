#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "index.h"

#define DHTTYPE DHT11
#define DHTPIN 4

int fan_value = 0;
const char* ssid = "espnet";
const char* password = "WIFIPASSWORD";
 
DHT dht(DHTPIN, DHTTYPE);
ESP8266WebServer server(80); //Server on port 80

void handleRoot() {
 String s = INDEX_page;
 server.send(200, "text/html", s); //Send web page
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.print("Connecting to wifi ");
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  

  server.on("/", handleRoot);      
  server.begin();
  Serial.println("HTTP server started");
  
  if (!MDNS.begin("ecu")) {             // Start the mDNS responder for esp8266.local
    Serial.println("Error setting up MDNS responder!");
  }
  Serial.println("mDNS responder started");

  analogWriteFreq(25000);
}

void loop() {
  server.handleClient();          //Handle client requests
 
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
  } else {
    // Compute heat index in Fahrenheit (the default)
    float hif = dht.computeHeatIndex(f, h);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);

    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("°C "));
    Serial.print(f);
    Serial.print(F("°F  Heat index: "));
    Serial.print(hic);
    Serial.print(F("°C "));
    Serial.print(hif);
    Serial.println(F("°F"));
  }

  int gas_value = analogRead(A0);
  Serial.print("Gas value: ");
  Serial.println(gas_value);
  
  //fan_value += 1023;
  //fan_value %= 1023;
  //analogWrite(5, fan_value);


  delay(2000);
}
