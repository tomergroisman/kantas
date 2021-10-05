#include <Kantas.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>

#define ledPin 2

Kantas::Kantas(char* ssid, char* password, int baud) {
  _ssid = ssid;
  _password = password;
  _baud = baud;
  server = 0;
}

Kantas::Kantas(char* ssid, char* password, int port, int baud) {
  _ssid = ssid;
  _password = password;
  _port = port;
  _baud = baud;
  server = std::make_unique<ESP8266WebServer>(port);
}

void Kantas::connect() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(_baud);

  WiFi.mode(WIFI_STA);
  WiFi.begin(_ssid, _password);

  Serial.println();
  Serial.print("Wait for WiFi");

  while (!this->isConnected()) {
    boolean isLedOn = digitalRead(ledPin);
    digitalWrite(ledPin, !isLedOn);
    Serial.print('.');
    delay(500);
  }
  digitalWrite(ledPin, HIGH);

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("Connected to ");
  Serial.println(_ssid);
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  Serial.println();
  delay(500);
}

void Kantas::startServer() {
  String msg = "HTTP server started";

  if (server) {
    if (MDNS.begin("esp8266")) {
      Serial.println("MDNS responder started");
    }
    server->begin();
  }
  else {
    msg = "ERROR: Server was not configured";
  }

  Serial.println(msg);
  Serial.println();
  delay(500);
}

void Kantas::update() {
  server->handleClient();
  MDNS.update();
}

String Kantas::get(String url) {
  String payload = "";

  if (this->isConnected()) {

    WiFiClient client;
    HTTPClient http;

    if (http.begin(client, url)) {

      int httpCode = http.GET();

      if (httpCode > 0) {

        this->_printMessageHeader("HTTP", "GET", url);
        Serial.printf("code: %d\n", httpCode);

        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          payload = http.getString();
        }
      } else {
        this->_printMessageHeader("HTTP", "GET", url);
        Serial.printf("failed! error: %s\n", http.errorToString(httpCode).c_str());      }

      http.end();
    } else {
      this->_printMessageHeader("HTTP", "GET", url);
      Serial.printf("Unable to connect\n");
    }

    return payload;
  }
}

String Kantas::get(String url, char fingerprint[]) {
  String payload = "";

  if (this->isConnected()) {


    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    HTTPClient https;

    client->setFingerprint(fingerprint);

    if (https.begin(*client, url)) {

      int httpCode = https.GET();

      if (httpCode > 0) {

        this->_printMessageHeader("HTTPS", "GET", url);
        Serial.printf("code: %d\n", httpCode);

        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          payload = https.getString();
        }
      } else {
        this->_printMessageHeader("HTTPS", "GET", url);
        Serial.printf("failed! error: %s\n", https.errorToString(httpCode).c_str());
      }

      https.end();
    } else {
      this->_printMessageHeader("HTTPS", "GET", url);
      Serial.printf("Unable to connect\n");
    }

    return payload;
  }
}

boolean Kantas::post(String url, String body) {
  if (this->isConnected()) {

    WiFiClient client;
    HTTPClient http;

    if (http.begin(client, url)) {

      http.addHeader("Content-Type", "application/json");
      int httpCode = http.POST(body);

      if (httpCode > 0) {

        this->_printMessageHeader("HTTPS", "POST", url);
        Serial.printf("code: %d\n", httpCode);

        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
        }
      } else {
        this->_printMessageHeader("HTTPS", "POST", url);
        Serial.printf("failed! error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
      return true;

    } else {

      this->_printMessageHeader("HTTPS", "POST", url);
      Serial.printf("Unable to connect\n");
      return false;

    }
  }
}


boolean Kantas::post(String url, char fingerprint[], String body) {
  if (this->isConnected()) {

    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    HTTPClient https;
    
    client->setFingerprint(fingerprint);

    if (https.begin(*client, url)) {

      https.addHeader("Content-Type", "application/json");
      int httpCode = https.POST(body);

      if (httpCode > 0) {

        this->_printMessageHeader("HTTPS", "POST", url);
        Serial.printf("code: %d\n", httpCode);

        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = https.getString();
        }
      } else {
        this->_printMessageHeader("HTTPS", "POST", url);
        Serial.printf("failed! error: %s\n", https.errorToString(httpCode).c_str());
      }

      https.end();
      return true;

    } else {

      this->_printMessageHeader("HTTPS", "POST", url);
      Serial.printf("Unable to connect\n");
      return false;

    }
  }
}

boolean Kantas::isConnected() {
  return WiFi.status() == WL_CONNECTED;
}

String Kantas::getIP() {
  return WiFi.localIP().toString();
}

void Kantas::_printMessageHeader(String protocol, String method, String url) {
  Serial.print("[");
  Serial.print(protocol);
  Serial.print("] ");
  Serial.print(method);
  Serial.print(" ");
  Serial.print(url);
  Serial.print("... ");
}
