/**
 * @Author: Michael K. Steinberg
 * @Created: 20/08/2022
 * @Purpose: Setup and general running of controller.
 */
#include <Arduino.h>
#include "config.h"
#include "wifi.h"
#include "http.h"
#include "rgbled.h"
#include "http_rgb.h"

void(* resetFunc) (void) = 0;


void setup() {
  Serial.begin(115200);

  (void)WIFI__init(g_NEW_HOST_NAME);

  g_http_server.appendHandler("/Control/Restart", resetFunc);

  g_http_server.appendHandler("/LED/Set", &HTTPRGB__setLed);
  g_http_server.appendHandler("/LED/Fill", &HTTPRGB__fillLeds);
  g_http_server.appendHandler("/LED", &HTTPRGB__state);
  g_http_server.appendHandler("/LED/Config", &HTTPRGB__config);
}

void loop() {
  g_http_server.loop();
  g_rgbled.loop();
}
