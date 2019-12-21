/*
    This sketch sends a string to a TCP server, and prints a one-line response.
    You must run a TCP server in your local network.
    For example, on Linux you can use this command: nc -v -l 3000
*/

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#ifndef STASSID
#define STASSID "COBOX"
#define STAPSK  "cobox_pass"
#endif
#include "DHTesp.h"
DHTesp dht;

const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "192.168.9.1";
const uint16_t port = 4444;
ESP8266WiFiMulti WiFiMulti;

void setup() {
  Serial.begin(115200);
  dht.setup(4, DHTesp::DHT22);
  // We start by connecting to a WiFi network
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");

  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(500);
}


void loop() {
  Serial.print("connecting to ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    Serial.println("wait 5 sec...");
    delay(5000);
    return;
  }
  float h = dht.getHumidity();
  float t = dht.getTemperature();
  Serial.println("temp : ");
  Serial.print(t);
  Serial.println("hum : ");
  Serial.print(h);
  
  // This will send the request to the server
  client.println((String)"1;"+t+";"+h+"\r");

  delay(5000);
  //read back one line from server
  Serial.println("receiving from remote server");
  String line = client.readStringUntil('\r');
  String temp = line.substring(0,1);
  int type = temp.toInt();
  temp = line.substring(2,3);
  int id_device = temp.toInt();
  temp = line.substring(4,5);
  int order = temp.toInt();
  
  Serial.println(line);
  Serial.print("Type : ");
  Serial.print(type);
  Serial.print(" Devide id : ");
  Serial.print(id_device);
  Serial.print(" Order : ");
  Serial.println(order);
  
  
  client.println(":exit");

  Serial.println("closing connection");
  
  client.stop();

  Serial.println("wait 5 sec...");
  delay(5000);
}
