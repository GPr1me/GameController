#ifndef ANIMATION_LAYER_H
#define ANIMATION_LAYER_H

#include "Models/WS2812B_matrix/WS2812BMatrix.h"

class AnimationLayer {
public:
    AnimationLayer(WS2812BMatrix& matrix);

    void begin();
    void run();

    // Animation methods
    void scrollText(const char* text, uint32_t color, uint32_t speed);
    void blinkText(const char* text, uint32_t color, uint32_t interval);
    void showScore(int score, uint32_t color = 0xFFFF00);
    void clear();

private:
    WS2812BMatrix& matrix;
    const char* currentText;
    uint32_t currentColor;
    uint32_t animationSpeed;
    uint32_t lastUpdateTime;
    uint32_t initialTime;
    bool isBlinking;
    bool isTextVisible;
    bool isTextScrolling;

    int currentScore;
    bool isScoreAnimating;
    uint32_t shineStep;

    void updateScroll();
    void updateBlink();
    void updateScore();
};

#endif // ANIMATION_LAYER_H