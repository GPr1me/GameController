#include <Arduino.h>
#include "Models/Button/Button.h"
#include "Models/WS2812B_matrix/WS2812BMatrix.h"
#include "Models/AnimationLayer/AnimationLayer.h"
#include "Config/WiFiConfig.h"
#include "WiFi.h"
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "Models/SignalR/SignalRClient.h"

using namespace websockets;

#define MATRIX_WIDTH 16
#define MATRIX_HEIGHT 8

#define MATRIX_PIN 17
#define BUTTON_1 14
#define BUTTON_2 27
#define BUTTON_3 26
#define BUTTON_4 25
#define BUTTON_5 33
#define BUTTON_6 32

WiFiClientSecure client;
HTTPClient http;
// WebsocketsClient websocket;

WS2812BMatrix matrix(MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_PIN);
AnimationLayer animationLayer(matrix);
uint32_t color = 0xF0F0F0;
JsonDocument doc;

SignalRClient signalRClient("192.168.40.52/gamehub");

void buttonCallback(const char* text, uint32_t color) {
    matrix.print(text, color);
    signalRClient.sendMessage("SendMessage", text);
}

ushort score = 0;


Button button1(BUTTON_1, [] (Button& button) { buttonCallback("A", 0xff4000); });
Button button2(BUTTON_2, [] (Button& button) { buttonCallback("B", 0x0000ff); });
Button button3(BUTTON_3, [] (Button& button) { buttonCallback("C", 0xffff00); });
Button button4(BUTTON_4, [] (Button& button) { buttonCallback("D", 0x00ff00); });
Button button5(BUTTON_5, [] (Button& button) { buttonCallback("E", 0xffffff); });
Button button6(BUTTON_6, [] (Button& button) { buttonCallback("F", 0x808080); });

void setup() {
    Serial.begin(9600);

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWi-Fi connected");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    client.setCACert(ca);
    if (!client.connect(server, port)) {
        Serial.println("Connection failed!");
    }

    signalRClient.begin();

    // Register callbacks for specific SignalR methods
    signalRClient.On("ScoreUpdated", [](String msg) {
        deserializeJson(doc, msg);
        score = doc["args"];

        animationLayer.showScore(score);
    });

    // signalRClient.On<String>("AnotherMethod", [](const String& arg) {
    //     Serial.println("Callback for AnotherMethod invoked with argument: " + arg);
    // });

    Serial.println("WebSocket client configured.");

    // Matrix setup
    matrix.begin();
    matrix.setBrightness(50);
    matrix.clear();
    delay(100);

    matrix.displayScrollingText("Marcotendo", color, 40);

    // buttons
    button1.begin();
    button2.begin();
    button3.begin();
    button4.begin();
    button5.begin();
    button6.begin();
    attachInterrupt(digitalPinToInterrupt(BUTTON_1), []() { button1.handleToggle(); }, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_2), []() { button2.handleToggle(); }, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_3), []() { button3.handleToggle(); }, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_4), []() { button4.handleToggle(); }, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_5), []() { button5.handleToggle(); }, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_6), []() { button6.handleToggle(); }, FALLING);

    Serial.println("****************** SETUP COMPLETE ******************");
}

void loop() {

    animationLayer.run();
    signalRClient.poll();

    // Button handling
    button1.run();
    button2.run();
    button3.run();
    button4.run();
    button5.run();
    button6.run();

    delay(10); // Add a small delay to reduce CPU usage
}
