#include "Kantas.h"

char* ssid = "*ssid*";
char* password = "*password*";
int port = 80;
int baud = 115200;

Kantas kantas(ssid, password, port, baud);

void setup() {

  kantas.connect();

  //  Use '->' notation on kantas.server since kants.server is a pointer
  kantas.server->on("/", HTTP_GET, handleRoot);
  kantas.server->onNotFound(handleNotFound);

  kantas.startServer();
  
}

void loop() {

  kantas.update();

}

void handleRoot() {
  String clientIp = kantas.server->client().remoteIP().toString();
  Serial.print("Got a GET request from: ");
  Serial.println(clientIp);
  
  if (kantas.server->args() > 0) {
    Serial.print("Request query params: ");
    Serial.println(getArgsStr());
  }
  
  kantas.server->send(200, "text/plain", "hello from Kantas server on esp8266!\r\n");
}

void handleNotFound() {
  String message = "URI: ";
  message += kantas.server->uri();
  message += "\nMethod: ";
  message += (kantas.server->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += kantas.server->args();
  message += "\n";
  message += getArgsStr();
  kantas.server->send(404, "text/plain", message);
}

String getArgsStr() {
  String message = "";
  
  for (uint8_t i = 0; i < kantas.server->args(); i++) {
    message += kantas.server->argName(i) + ": ";
    message += kantas.server->arg(i) + "\n";
  }

  return message;
}
