#include "DHTesp.h" // Click here to get the library: http://librarymanager/All#DHTesp
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifdef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP8266 ONLY!)
#error Select ESP8266 board.
#endif

#ifndef STASSID
#define STASSID "HomeWiFi"
#define STAPSK "mas113355"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

DHTesp dht;
String casos[8] = {"Muy Seco", "Conforable", "Agradable", "bien", "desconforme", "bien desconforme", "pesimo", "horrible"};


void handleRoot() {
  delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  byte perception = dht.computePerception(temperature, humidity);

  String mensaje = "Humedad: ";
  mensaje+=humidity;
  mensaje+=" % Temperatura: ";
  mensaje+=temperature;
  mensaje+=" °C Estado: ";
  mensaje+=casos[perception];
  mensaje+=" Estado Sensor: ";
  mensaje+=dht.getStatusString();
  server.send(200, "text/plain", mensaje);
}

void setup()
{
  Serial.begin(115200);
   dht.setup(4, DHTesp::DHT11); 

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  if (MDNS.begin("esp8266")) { Serial.println("MDNS responder started"); }

  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  server.handleClient();
  MDNS.update();
  delay(2000);
}