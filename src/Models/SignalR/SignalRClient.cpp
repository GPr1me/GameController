#include "SignalRClient.h"

SignalRClient::SignalRClient(const String& serverUrl)
    : _serverUrl(serverUrl) {}

void SignalRClient::begin() {
    negotiate();
    _websocket.onMessage([this](WebsocketsMessage message) { onMessageCallback(message); });
    _websocket.onEvent([this](WebsocketsEvent event, String data) { onEventsCallback(event, data); });
}

void SignalRClient::negotiate() {
    String negotiateUrl = "http://" + _serverUrl + "/negotiate?negotiateVersion=1";
    http.begin(negotiateUrl);
    int httpCode = http.POST("");

    if (httpCode > 0) {
        String payload = http.getString();
        Serial.println("Negotiate response: " + payload);

        DeserializationError error = deserializeJson(_doc, payload);

        if (error) {
            Serial.print("Failed to parse JSON: ");
            Serial.println(error.c_str());
        } else {
            _connectionToken = String(_doc["connectionToken"].as<const char*>());
            String websocketUrl = "ws://" + _serverUrl + "?id=" + _connectionToken;
            Serial.println("WebSocket URL: " + websocketUrl);

            _websocket.connect(websocketUrl);
            sendHandshake();
        }
    } else {
        Serial.println("Negotiate request failed");
    }

    http.end();
}

void SignalRClient::sendHandshake() {
    String handshakeMessage = "{\"protocol\":\"json\",\"version\":1}\x1E";
    _websocket.send(handshakeMessage);
    Serial.println("SignalR handshake sent.");
}

void SignalRClient::sendMessage(const String& methodName, const String& argument) {
    String payload = "{\"type\":1,\"target\":\"" + methodName + "\",\"arguments\":[\"" + argument + "\"]}\x1E";
    _websocket.send(payload);
    Serial.println("SignalR method called: " + methodName + " with argument: " + argument);
}

void SignalRClient::poll() {
    _websocket.poll();
}

void SignalRClient::On(const String& methodName, std::function<void(const String&)> callback) {
    _callbacks[methodName] = callback;
}

void SignalRClient::onMessageCallback(WebsocketsMessage message) {
    Serial.println("Received message: " + message.data());

    // Parse the incoming message
    DeserializationError error = deserializeJson(_doc, message.data());

    if (error) {
        Serial.print("Failed to parse incoming message: ");
        Serial.println(error.c_str());
        return;
    }

    // Check if the message is a SignalR invocation
    if (_doc["type"] == 1) {
        String target = _doc["target"].as<String>();
        JsonArray arguments = _doc["arguments"];

        if (_callbacks.find(target) != _callbacks.end()) {
            // Invoke the registered callback with the first argument
            if (!arguments.isNull() && arguments.size() > 0) {
                
                _callbacks[target](arguments[0].as<String>());
            }
        }
    }
}

void SignalRClient::onEventsCallback(WebsocketsEvent event, String data) {
  switch (event) {
    case WebsocketsEvent::ConnectionOpened:
      Serial.println("WebSocket connection opened.");
      _sendHandshake();
      break;
    case WebsocketsEvent::ConnectionClosed:
      Serial.println("WebSocket connection closed.");
      begin();
      break;
    case WebsocketsEvent::GotPing:
      Serial.println("WebSocket ping received.");
      break;
    case WebsocketsEvent::GotPong:
      Serial.println("WebSocket pong received.");
      break;
    default:
      break;
  }
}
