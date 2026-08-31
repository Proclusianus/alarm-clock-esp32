#include "wifiHandling.h"

void enable_WiFi()
{
  const char* ssid       = "";
  const char* password   = "";
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");
}

void disable_WiFi()
{
  WiFi.disconnect(true);
  //WiFi.mode(WIFI_OFF);
  Serial.println("WiFi DISCONNECTED");
}