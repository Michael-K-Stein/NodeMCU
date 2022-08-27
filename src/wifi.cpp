#include "wifi.h"

ReturnCode WIFI__init(const char* host_name) 
{
  WiFi.mode(WIFI_STA);

  //Get Current Hostname
  Serial.printf("Default hostname: %s\n", WiFi.hostname().c_str());

  //Set new hostname
  WiFi.hostname(host_name);

  //Get Current Hostname
  Serial.printf("New hostname: %s\n", WiFi.hostname().c_str());
  
  //Init Wi-Fi
  WiFi.begin(g_WIFI_SSID, g_WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(250);
  }
  
  Serial.println("");
  Serial.println(WiFi.localIP());
  Serial.print("RSSI: ");
  Serial.println(WiFi.RSSI());

  return ReturnCode::Success;
}