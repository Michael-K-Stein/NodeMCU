/**
 * @Author: Michael K. Steinberg
 * @Created: 26/08/2022
 * @Purpose: Definitions for a FastLED wrapper more suited for Wireless control.
 */
#include "rgbled.h"

// Blend one CRGB color toward another CRGB color by a given amount.
// Blending is linear, and done in the RGB color space.
// This function modifies 'cur' in place.
CRGB fadeTowardColor( CRGB& cur, const CRGB& target, uint8_t amount)
{
  nblendU8TowardU8( cur.red,   target.red,   amount);
  nblendU8TowardU8( cur.green, target.green, amount);
  nblendU8TowardU8( cur.blue,  target.blue,  amount);
  return cur;
}

// Helper function that blends one uint8_t toward another by a given amount
void nblendU8TowardU8( uint8_t& cur, const uint8_t target, uint8_t amount)
{
  if( cur == target) return;
  
  if( cur < target ) {
    uint8_t delta = target - cur;
    delta = scale8_video( delta, amount);
    cur += delta;
  } else {
    uint8_t delta = cur - target;
    delta = scale8_video( delta, amount);
    cur -= delta;
  }
}

CRGB spectrumColor(uint32_t x)
{
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;

    float w = (uint8_t)x;
    w /= 0xff;
    w *= 100.0;

    if (w < 17) {
        r = -(w - 17.0f) / 17.0f;
        b = 1.0f;
    } else if (w < 33) {
        g = (w - 17.0f) / (33.0f - 17.0f);
        b = 1.0f;
    } else if (w < 50) {
        g = 1.0f;
        b = -(w - 50.0f) / (50.0f - 33.0f);
    } else if (w < 67) {
        r = (w - 50.0f) / (67.0f - 50.0f);
        g = 1.0f;
    } else if (w < 83) {
        r = 1.0f;
        g = -(w - 83.0f) / (83.0f - 67.0f);
    } else {
        r = 1.0f;
        b = (w - 83.0f) / (100.0f - 83.0f);
    }

    return CRGB((int)(r * 255), (int)(g * 255), (int)(b * 255));
}

RGBLed::RGBLed(const int ledCount) 
    : m_speed(20), m_hue(0), m_ledCount(ledCount), m_leds(nullptr), m_state(RGBLedState::Normal)
{
    m_leds = new CRGB[m_ledCount];
    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(m_leds, m_ledCount).setCorrection(TypicalLEDStrip);

    fillLeds(CRGB(0));
}

RGBLed::~RGBLed() 
{
    delete m_leds;
}

void RGBLed::loop() 
{
    switch (m_state) 
    {
    case RGBLedState::Disabled:
        break;
    case RGBLedState::Normal:
        FastLED.show();
        break;
    case RGBLedState::Fade:
        doFadeLoop();
        break;
    case RGBLedState::Rainbow:
        doRainbowLoop();
        break;
    }
}

void RGBLed::doFadeLoop() 
{
    fillLeds(spectrumColor(m_hue));
    EVERY_N_MILLISECONDS(m_speed) { ++m_hue; }
}

void RGBLed::doRainbowLoop() 
{
    fill_rainbow(m_leds, m_ledCount, m_hue, 7);
    EVERY_N_MILLISECONDS(m_speed) { ++m_hue; }
    FastLED.show();
}

void RGBLed::show()
{
    FastLED.show();
}

int RGBLed::getBrightness() 
{
    return FastLED.getBrightness();
}

void RGBLed::setBrightness(int value) 
{
    FastLED.setBrightness(value);
}

void RGBLed::setLed(const size_t ledIndex, const CRGB& color, bool refresh) 
{
    m_leds[ledIndex] = color;

    if (refresh) 
    {
        show();
    }
}

void RGBLed::fillLeds(const CRGB& color, bool refresh) 
{
    for (size_t i = 0; i < m_ledCount; ++i) 
    {
        setLed(i, color, false);
    }

    if (refresh) 
    {
        show();
    }
}

void RGBLed::startFade() 
{
    m_state = RGBLedState::Fade;
}

void RGBLed::stopFade() 
{
    m_state = RGBLedState::Disabled;
}

void RGBLed::startRainbow() 
{
    m_state = RGBLedState::Rainbow;
}

void RGBLed::stopRainbow() 
{
    m_state = RGBLedState::Disabled;
}

String RGBLed::stringifyState(RGBLedState state) 
{
    switch (state) 
    {
    case RGBLedState::Disabled:
        return "Disabled";
        break;
    case RGBLedState::Normal:
        return "Normal";
        break;
    case RGBLedState::Fade:
        return "Fade";
        break;
    case RGBLedState::Rainbow:
        return "Rainbow";
        break;
    };

    return "Error!";
}

void RGBLed::setState(RGBLedState state) 
{ 
    m_state = state;
}

RGBLedState RGBLed::parseState(String state) 
{
    if (state.compareTo("Disabled") == 0) 
    {
        return RGBLedState::Disabled;
    }
    else if (state.compareTo("Normal") == 0) 
    {
        return RGBLedState::Normal;
    }
    else if (state.compareTo("Rainbow") == 0) 
    {
        return RGBLedState::Rainbow;
    }
    else if (state.compareTo("Fade") == 0) 
    {
        return RGBLedState::Fade;
    }

    return RGBLedState::Disabled;
}
