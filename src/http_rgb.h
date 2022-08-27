/**
 * @Author: Michael K. Steinberg
 * @Created: 27/08/2022
 * @Purpose: A library to cross between a HTTP server and RGBleds.
 */
#ifndef _HTTP_RGB_H
#define _HTTP_RGB_H

#include "common.h"
#include "http.h"
#include "rgbled.h"

extern HTTPServer g_http_server;
extern RGBLed g_rgbled;

String buildHTMLColorPreview(const CRGB& color);
ReturnCode parseRGBColor(CRGB& color);
ReturnCode parseLEDState(RGBLedState& state) ;
ReturnCode parseSpeed(size_t& speed) ;
ReturnCode parseHue(uint32_t& hue) ;
void HTTPRGB__setLed() ;
void HTTPRGB__fillLeds() ;
void HTTPRGB__state() ;
void HTTPRGB__config() ;

#endif // _HTTP_RGB_H
