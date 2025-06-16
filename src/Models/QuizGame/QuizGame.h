#ifndef QUIZ_GAME_H
#define QUIZ_GAME_H

#include <Arduino.h>
#include "../Program/Program.h"
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

class QuizGame : public Program {
public:
    QuizGame(WS2812BMatrix* matrix, 
             AnimationLayer* animationLayer, 
             Button* button1, 
             Button* button2, 
             Button* button3, 
             Button* button4, 
             Button* button5, 
             Button* button6);

    ~QuizGame();
    void setup() override;
    void run() override;
    void drawTitleScreen(bool isSelected) override;

    bool isSetup() override;

private:
    void _buttonCallback(const char* text, uint16_t color);
    void _setupSignalRClient();
    void _setupButtons();

    WS2812BMatrix* _matrix;
    AnimationLayer* _animationLayer;
    SignalRClient* _signalRClient;

    Button* _button1;
    Button* _button2;
    Button* _button3;
    Button* _button4;
    Button* _button5;
    Button* _button6;

    JsonDocument _doc;
    WiFiClientSecure _client;
    HTTPClient _http;

    ushort _score;
    String _name;
};

#endif // QUIZ_GAME_H