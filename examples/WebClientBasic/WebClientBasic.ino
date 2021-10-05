#include "Kantas.h"

char* ssid = "*ssid*";
char* password = "*password*";
int baud = 115200;

char httpUrl[] = "http://info.cern.ch";
char httpsUrl[] = "https://google.com";
char fingerprint[] = "11:B6:60:D8:58:65:E6:26:EC:67:61:3A:2A:F4:89:E4:A6:DE:B3:44";

boolean isFetched = false;

Kantas kantas(ssid, password, baud);

void setup() {

  kantas.connect();
  
}

void loop() {

  if (!isFetched) {

    Serial.print("Try to get http url ");
    Serial.print(httpUrl);
    Serial.println("...");
    
    delay(1000);
    
    String payload = kantas.get(httpUrl);
    if (payload.length() == 0) {
      return;
    }
    
    Serial.println("Payload:");
    Serial.println(payload);
    Serial.println();

    delay(1000);

    Serial.print("Try to get https url ");
    Serial.print(httpsUrl);
    Serial.println("...");
  
    delay(1000);
    
    payload = kantas.get(httpsUrl, fingerprint);  
    if (payload.length() == 0) {
      return;
    }

    Serial.println("Payload:");
    Serial.println(payload);

    Serial.println("done.");

    isFetched = true;
  
  }

}