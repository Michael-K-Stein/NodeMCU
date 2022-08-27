/**
 * @Author: Michael K. Steinberg
 * @Created: 20/08/2022
 * @Purpose: Definitions for a simple http server.
 */
#include "http.h"

ESP8266WebServer * m_http_server = NULL;
uint8_t ref_count = 0;

HTTPServer::HTTPServer() 
{
    if (0 == ref_count) 
    {
        ref_count = 1;

        m_http_server = new ESP8266WebServer();

        m_http_server->on("/", HTTP__handle_root);
        m_http_server->onNotFound(HTTP__handle_not_found);

        m_http_server->begin();
    }
}

HTTPServer::~HTTPServer() 
{
    if (NULL != m_http_server) 
    {
        delete m_http_server;
    }
    m_http_server = NULL;
    ref_count = 0;
}

ESP8266WebServer * HTTPServer::getRaw() 
{
    return m_http_server;
}

void HTTPServer::loop() 
{
    m_http_server->handleClient();
}

void HTTPServer::appendHandler(const char * url, HttpHandler handler) 
{
    m_http_server->on(url, handler);
}

void HTTPServer::send(int returnCode, const char* type, const char* content) 
{
    m_http_server->send(returnCode, type, content);
}

void HTTPServer::send(int returnCode, const char* type, String content) 
{
    m_http_server->send(returnCode, type, content);
}

void HTTPServer::send(int returnCode, const char* type, int content) 
{
    char buffer[32] = { 0 };
    (void)sprintf(buffer, "%d", content);
    m_http_server->send(returnCode, type, buffer);
}

void HTTP__handle_root() 
{
    char buffer[512] = { 0 };
    (void)sprintf(buffer, "%s", g_NEW_HOST_NAME);
    m_http_server->send(200, "text/plain", buffer);
}

void HTTP__handle_not_found() 
{
    m_http_server->send(404, "text/plain", "Page not found!");
}
