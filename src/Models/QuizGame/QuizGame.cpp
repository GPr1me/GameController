#include "QuizGame.h"

        
static const uint16_t logo8x5[8 * 5] = {
    BLACK,BLACK,BLACK,BLACK,BLACK,
    BLACK,WHITE,WHITE,WHITE,BLACK,
    WHITE,BLACK,BLACK,BLACK,WHITE,
    BLACK,BLACK,BLACK,BLACK,WHITE,
    BLACK,BLACK,BLACK,WHITE,BLACK,
    BLACK,BLACK,WHITE,BLACK,BLACK,
    BLACK,BLACK,BLACK,BLACK,BLACK,
    BLACK,BLACK,WHITE,BLACK,BLACK
};

QuizGame::QuizGame(WS2812BMatrix* matrix, 
                   AnimationLayer* animationLayer, 
                   Button* button1, 
                   Button* button2, 
                   Button* button3, 
                   Button* button4, 
                   Button* button5, 
                   Button* button6) : _matrix(matrix),
                                      _animationLayer(animationLayer), 
                                      _button1(button1),
                                      _button2(button2),
                                      _button3(button3),
                                      _button4(button4),
                                      _button5(button5),
                                      _button6(button6)
{
    _signalRClient = new SignalRClient("192.168.40.52/gamehub");
    _score = 0;
    _name = "";
    _isSetup = false;
}
              
QuizGame::~QuizGame()
{
    if (!_signalRClient) return;
    
    delete _signalRClient;
    _signalRClient = nullptr;
}

void QuizGame::setup()
{
	_animationLayer->animateMenuTitleBlinkAndGrow(logo8x5, 5, 8, CHARTREUSE, BLUE, 6, 8, 100, 4);
    _animationLayer->clear();

    _setupSignalRClient();
    _setupButtons();

    _isSetup = true;
}

void QuizGame::run()
{
    if (_isSetup)
    {
        _signalRClient->poll();
    }

    _button1->run();
    _button2->run();
    _button3->run();
    _button4->run();
    _button5->run();
    _button6->run();
    _animationLayer->run();
}

bool QuizGame::isSetup() { return _signalRClient != nullptr && Program::isSetup(); }

void QuizGame::drawTitleScreen(bool isSelected) {
    uint16_t bandColor = isSelected ? CHARTREUSE : BLACK;
    _animationLayer->drawMenuTitle(logo8x5, 5, 8, bandColor, BLUE, 2);

}

void QuizGame::_setupSignalRClient() {
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

    _client.setCACert(ca);
    if (!_client.connect(server, port)) {
        Serial.println("Connection failed!");
    }

    _signalRClient->begin();

    _signalRClient->On("ScoreUpdated", [this](String msg)
    {
        deserializeJson(this->_doc, msg);

        this->_score = this->_doc["args"];
        this->_animationLayer->showScore(this->_score);
    });

    _signalRClient->On("YourName", [this](String msg)
    {
        deserializeJson(this->_doc, msg);

        this->_name = this->_doc["Name"].as<const char*>();
        this->_animationLayer->clear();
        this->_animationLayer->scrollText(this->_name.c_str(), 0xF0F0F0, 50);
    });
}

void QuizGame::_setupButtons() {
    _button1->setCallback([this](Button& button)
    {
        this->_animationLayer->clear();
        this->_buttonCallback("A", RED);
    });
    _button1->setDebounceDelay(100);
    _button2->setCallback([this](Button& button)
    {
        this->_animationLayer->clear();
        this->_buttonCallback("B", BLUE);
    });
    _button2->setDebounceDelay(100);
    _button3->setCallback([this](Button& button)
    {
        this->_animationLayer->clear();
        this->_buttonCallback("C", YELLOW);
    });
    _button3->setDebounceDelay(100);
    _button4->setCallback([this](Button& button)
    {
        this->_animationLayer->clear();
        this->_buttonCallback("D", GREEN);
    });
    _button4->setDebounceDelay(100);
    _button5->setCallback([this](Button& button)
    {
        this->_animationLayer->clear();
        this->_animationLayer->scrollText(this->_name.c_str(), WHITE, 50);
    });
    _button5->setDebounceDelay(100);
    _button6->setCallback([this](Button& button)
    {
        this->_animationLayer->clear();
        this->_animationLayer->showScore(this->_score);
    });
    _button6->setDebounceDelay(100);
}

void QuizGame::_buttonCallback(const char* text, uint16_t color) {
    _matrix->print(text, color);
    _signalRClient->sendMessage("SendMessage", text);
}