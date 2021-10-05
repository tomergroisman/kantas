#include "Kanta.h"

char* ssid = "*ssid*";
char* password = "*password*";
int port = 80;
int baud = 115200;

Kanta kanta(ssid, password, port, baud);

void setup() {

  kanta.connect();

  //  Use '->' notation on kanta.server since kants.server is a pointer
  kanta.server->on("/", HTTP_GET, handleRoot);
  kanta.server->onNotFound(handleNotFound);

  kanta.startServer();
  
}

void loop() {

  kanta.update();

}

void handleRoot() {
  String clientIp = kanta.server->client().remoteIP().toString();
  Serial.print("Got a GET request from: ");
  Serial.println(clientIp);
  
  if (kanta.server->args() > 0) {
    Serial.print("Request query params: ");
    Serial.println(getArgsStr());
  }
  
  kanta.server->send(200, "text/plain", "hello from esp8266!\r\n");
}

void handleNotFound() {
  String message = "URI: ";
  message += kanta.server->uri();
  message += "\nMethod: ";
  message += (kanta.server->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += kanta.server->args();
  message += "\n";
  message += getArgsStr();
  kanta.server->send(404, "text/plain", message);
}

String getArgsStr() {
  String message = "";
  
  for (uint8_t i = 0; i < kanta.server->args(); i++) {
    message += kanta.server->argName(i) + ": ";
    message += kanta.server->arg(i) + "\n";
  }

  return message;
}
