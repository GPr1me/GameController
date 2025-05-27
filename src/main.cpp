#include <Arduino.h>
#include "Models/Button/Button.h"
#include "Models/WS2812B_matrix/WS2812BMatrix.h"
#include "Models/AnimationLayer/AnimationLayer.h"
#include "Models/QuizGame/QuizGame.h"
#include "Models/Tetris/Tetris.h"
#include "Models/Menu/Menu.h"

#define MATRIX_WIDTH 16
#define MATRIX_HEIGHT 8

#define MATRIX_PIN 17
#define BUTTON_1 14
#define BUTTON_2 27
#define BUTTON_3 26
#define BUTTON_4 25
#define BUTTON_5 33
#define BUTTON_6 32

WS2812BMatrix matrix(MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_PIN);
WS2812BMatrix invertedMatrix(MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_PIN, true);
AnimationLayer animationLayer(matrix);


void buttonCallback(const char* text, uint16_t color) {
    matrix.print(text, color);
    Serial.println(text);
}

Button button1(BUTTON_1, [] (Button& button) { buttonCallback("A", RED); });
Button button2(BUTTON_2, [] (Button& button) { buttonCallback("B", BLUE); });
Button button3(BUTTON_3, [] (Button& button) { buttonCallback("C", YELLOW); });
Button button4(BUTTON_4, [] (Button& button) { buttonCallback("D", GREEN); });
Button button5(BUTTON_5, [] (Button& button) { buttonCallback("E", 0x4041); });
Button button6(BUTTON_6, [] (Button& button) { buttonCallback("F", WHITE); });

QuizGame quizGame(&matrix, &animationLayer, &button1, &button2, &button3, &button4, &button5, &button6);
Tetris tetrisGame(&invertedMatrix, &animationLayer, &button5, &button6, &button1, &button2);
Menu menu(&button1, &button2, &button6, &button5, &animationLayer, &matrix, &quizGame, &tetrisGame);

void setup() {
    Serial.begin(9600);

    // Matrix setup
    matrix.begin();
    matrix.setBrightness(50);
    matrix.clear();
    invertedMatrix.begin();
    invertedMatrix.setBrightness(50);
    invertedMatrix.clear();
    delay(100);

    animationLayer.begin();

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

    // Menu
    menu.setup();
    menu.drawTitleScreen(false);

    Serial.println("****************** SETUP COMPLETE ******************");
}

void loop() {

    menu.run();

    delay(10); // Add a small delay to reduce CPU usage
}
