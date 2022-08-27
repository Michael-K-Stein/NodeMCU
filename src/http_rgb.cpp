/**
 * @Author: Michael K. Steinberg
 * @Created: 27/08/2022
 * @Purpose: A library to cross between a HTTP server and RGBleds.
 */
#include "http_rgb.h"

HTTPServer g_http_server{ };
RGBLed g_rgbled{ LED_COUNT };

String buildHTMLColorPreview(const CRGB& color) 
{
  char buffer[8] = { 0 };
  (void)sprintf(buffer, "%.2hhX%.2hhX%.2hhX", color.r, color.g, color.b);

  return String("<input type='color' value='#") + buffer + String("'>");
}

ReturnCode parseRGBColor(CRGB& color) 
{
  if (g_http_server.getRaw()->arg("Hex") == "" && g_http_server.getRaw()->arg("RGB") == "") 
  {
    return ReturnCode::InvalidParameters;
  }
  else if (g_http_server.getRaw()->arg("Hex") != "")
  {
    // Hex parameter
    long hex_color = strtol(g_http_server.getRaw()->arg("Hex").c_str(), NULL, 16);
    color.blue = hex_color & 0xFF;
    color.green = (hex_color >> 8) & 0xFF;
    color.red = (hex_color >> 16) & 0xFF;
  }
  else if (g_http_server.getRaw()->arg("RGB") != "") 
  {
    sscanf(g_http_server.getRaw()->arg("RGB").c_str(), "%hhX,%hhX,%hhX", &color.red, &color.green, &color.blue);
  }
  return ReturnCode::Success;
}

ReturnCode parseLEDState(RGBLedState& state) 
{
  if (g_http_server.getRaw()->arg("State") == "") 
  {
    return ReturnCode::InvalidParameters;
  } 
  else 
  {
    state = g_rgbled.parseState(g_http_server.getRaw()->arg("State"));
  }
  return ReturnCode::Success;
}

ReturnCode parseSpeed(size_t& speed) 
{
  if (g_http_server.getRaw()->arg("Speed") == "") 
  {
    return ReturnCode::InvalidParameters;
  } 
  else 
  {
    speed = g_http_server.getRaw()->arg("Speed").toInt();
  }
  return ReturnCode::Success;
}

ReturnCode parseHue(uint32_t& hue) 
{
  if (g_http_server.getRaw()->arg("Hue") == "") 
  {
    return ReturnCode::InvalidParameters;
  } 
  else 
  {
    hue = g_http_server.getRaw()->arg("Hue").toInt();
  }
  return ReturnCode::Success;
}

String buildStateSelection() 
{
    String sel = "";
    String opt;

    sel += "<select name='State'>";
    for (int state = static_cast<int>(RGBLedState::Begin); state != static_cast<int>(RGBLedState::End); ++state) 
    {
        opt = RGBLed::stringifyState(static_cast<RGBLedState>(state));
        sel += "<option value='" + opt + "' " + ((g_rgbled.getState() == static_cast<RGBLedState>(state)) ? "selected" : "") + ">" + opt + "</option>";
    }
    sel += "</select>";

    return sel;
}

void HTTPRGB__setLed() 
{
  CRGB color = { 0 };
  size_t led_index = 0;
  String return_message = "";

  if (g_http_server.getRaw()->arg("Index") != "")
  {
    led_index = strtoll(g_http_server.getRaw()->arg("Index").c_str(), NULL, 16);
    if (parseRGBColor(color) == ReturnCode::Success) 
    {
      g_rgbled.setLed(led_index, color);
      return_message += buildHTMLColorPreview(color);
    } 
    else 
    {
      return_message += "Please add parameters 'Hex' or 'RGB'\n";
    }
  } 
  else 
  {
    return_message += "Please add 'Index' parameter\n";
  }

  g_http_server.send(200, "text/html", return_message);
}

void HTTPRGB__fillLeds() 
{
  CRGB color = { 0 };
  String return_message = "";

  if (parseRGBColor(color) == ReturnCode::Success) 
  {
    g_rgbled.fillLeds(color);
    return_message += buildHTMLColorPreview(color);
  } 
  else 
  {
    return_message += "Please add parameters 'Hex' or 'RGB'\n";
  }

  g_http_server.send(200, "text/html", return_message);
}

void HTTPRGB__state() 
{
  String return_message = "";
  RGBLedState state = RGBLedState::Disabled;

  if (parseLEDState(state) == ReturnCode::Success) 
  {
    g_rgbled.setState(state);
    return_message += "Changed led state\n";
  } 
  else 
  {
    return_message += "Failed to parse led state!\n";
  }

  g_http_server.send(200, "text/html", return_message);
}

void HTTPRGB__config() 
{
    String return_message = "";
    RGBLedState state = RGBLedState::Disabled;

    if (parseSpeed(g_rgbled.m_speed) == ReturnCode::Success) 
    {
        return_message += "<p>Changed speed</p>";
    } 
    if (parseHue(g_rgbled.m_hue) == ReturnCode::Success) 
    {
        return_message += "<p>Changed hue</p>";
    } 
    if (parseLEDState(state) == ReturnCode::Success) 
    {
        g_rgbled.setState(state);
        return_message += "<p>Changed state</p>";
    } 

    return_message += 
        "<form action='/LED/Config'>\
            State: " + buildStateSelection() + "<br>\
            Speed: <input name='Speed' type='text' value='" + String(g_rgbled.m_speed) + "'></input><br>\
            Hue: <input name='Hue' type='text' value='" + String(g_rgbled.m_hue) + "'></input><br>\
            <input type='Submit'></input>\
        </form><br>";

  g_http_server.send(200, "text/html", return_message);
}
