#ifndef SIGNALR_CLIENT_H
#define SIGNALR_CLIENT_H

#include <Arduino.h>
#include <ArduinoWebsockets.h>
#include <ArduinoJson.h>
#include <map>
#include <functional>

using namespace websockets;

class SignalRClient {
public:
    SignalRClient(const String& serverUrl);

    void begin();
    void poll();

    void On(const String& methodName, std::function<void(const String&)> callback);

    void sendMessage(const String& methodName, const String& argument);

private:
    String _serverUrl;
    String _connectionToken;
    WebsocketsClient _websocket;
    HTTPClient http;
    JsonDocument _doc;

    std::map<String, std::function<void(const String&)>> _callbacks;

    void negotiate();
    void sendHandshake();
    void onMessageCallback(WebsocketsMessage message);
    void onEventsCallback(WebsocketsEvent event, String data);
};

#endif // SIGNALR_CLIENT_H
