/*
  Kanta.h - Library for WiFi web client.
  Created by Tomer Groisman, September 12, 2021.
*/

#ifndef Kantas_h
#define Kantas_h

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

class Kantas {
  private:
    char* _ssid;
    char* _password;
    int _port;
    int _baud;
    void _printMessageHeader(String protocol, String method, String url);

  public:
    Kantas(char* ssid, char* password, int baud);
    Kantas(char* ssid, char* password, int port, int baud);
    void connect();
    void startServer();
    void update();
    String get(String url);
    String get(String url, char fingerprint[]);
    boolean post(String url, char fingerprint[], String body);
    boolean post(String url, String body);
    boolean isConnected();
    String getIP();

    std::unique_ptr<ESP8266WebServer> server;
};

#endif
