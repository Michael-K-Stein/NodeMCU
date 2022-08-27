/**
 * @Author: Michael K. Steinberg
 * @Created: 20/08/2022
 * @Purpose: Declerations for a simple http server.
 */
#ifndef __HTTP_H
#define __HTTP_H

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "config.h"

using HttpHandler = void();

extern ESP8266WebServer * m_http_server;
extern uint8_t ref_count;

class HTTPServer
{
public:
    HTTPServer();
    ~HTTPServer();

    ESP8266WebServer * getRaw();

    void loop();

    void appendHandler(const char * url, HttpHandler handler);

    void send(int, const char*, const char*);
    void send(int, const char*, String);
    void send(int, const char*, int);
};

void HTTP__handle_root();
void HTTP__handle_not_found();

#endif // _HTTP_H