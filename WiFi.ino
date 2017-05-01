#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "WiFiCreds.h"

void startWiFi() {
  // Initialize WiFi connection
  WiFi.begin(ssid, pass); //connecting to the router
  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}
