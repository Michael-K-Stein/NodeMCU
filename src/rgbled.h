/**
 * @Author: Michael K. Steinberg
 * @Created: 26/08/2022
 * @Purpose: Declerations for a FastLED wrapper more suited for Wireless control.
 */
#ifndef __RGBLED_H
#define __RGBLED_H

#include <FastLED.h>
#include "common.h"

#define LED_TYPE   WS2811
#define COLOR_ORDER   GRB
#define DATA_PIN        3
//#define CLK_PIN       4
#define VOLTS          12
#define MAX_MA       4000

enum class RGBLedState 
{
    Begin, 
    Disabled = Begin,
    Normal,
    Fade,
    Rainbow,
    End,
};

CRGB fadeTowardColor( CRGB& cur, const CRGB& target, uint8_t amount);
void nblendU8TowardU8( uint8_t& cur, const uint8_t target, uint8_t amount);

class RGBLed
{
public:
    RGBLed(int ledCount);
    ~RGBLed();

    void loop();

    void show();

    int getBrightness();
    void setBrightness(int);

    void setLed(size_t ledIndex, const CRGB& color, bool refresh = true);
    void fillLeds(const CRGB& color, bool refresh = true);

    void startFade();
    void stopFade();

    void startRainbow();
    void stopRainbow();

    static String stringifyState(RGBLedState state);
    RGBLedState getState() { return m_state; }
    void setState(RGBLedState state);
    static RGBLedState parseState(String state);

    size_t m_speed;
    uint32_t m_hue;

private:
    void doFadeLoop();
    void doRainbowLoop();

    size_t m_ledCount;
    CRGB * m_leds;
    RGBLedState m_state;

};

#endif // __RGBLED_H
