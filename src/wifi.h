/**
 * @Author: Michael K. Steinberg
 * @Created: 26/08/2022
 * @Purpose: Declerations for wifi setup and control.
 */
#ifndef __WIFI_H
#define __WIFI_H

#include <ESP8266WiFi.h>
#include <string_view>
#include "common.h"

ReturnCode WIFI__init(const char* host_name);

#endif // __WIFI_H