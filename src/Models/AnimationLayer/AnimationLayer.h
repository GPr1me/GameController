#ifndef ANIMATION_LAYER_H
#define ANIMATION_LAYER_H

#include "Models/WS2812B_matrix/WS2812BMatrix.h"

class AnimationLayer {
public:
    AnimationLayer(WS2812BMatrix& matrix);

    void begin();
    void run();

    // Animation methods
    void animateMenuTitleBlinkAndGrow(const uint16_t* logo, uint8_t logoWidth, uint8_t logoHeight, uint16_t bandColor, uint16_t backgroundColor, uint8_t startWidth, uint8_t endWidth, uint16_t blinkDelay = 100, uint8_t blinkCount = 6);
    void blinkText(const char* text, uint32_t color, uint32_t interval);
    void scrollText(const char* text, uint32_t color, uint32_t speed);
    void clear();
    void drawMenuTitle(const uint16_t* logo, uint8_t logoWidth, uint8_t logoHeight, uint16_t bandColor, uint16_t backgroundColor, uint8_t bandWidth = 4);
    void showLoadingAnimation();
    void showScore(int score, uint32_t color = 0xFFFF00);

private:
    WS2812BMatrix& _matrix;

    const char* _currentText;

    uint32_t _animationSpeed;
    uint32_t _currentColor;
    uint32_t _initialTime;
    uint32_t _lastUpdateTime;

    enum AnimationType {
        NONE,
        SCROLLING_TEXT,
        BLINKING_TEXT,
        SCORE_ANIMATION,
        LOADING_ANIMATION
    };
    AnimationType _currentAnimationType;

    bool _isTextVisible;

    int _currentScore;
    uint32_t _shineStep;

    void _updateScroll();
    void _updateBlink();
    void _updateScore();
    void _updateLoading();
};

#endif // ANIMATION_LAYER_H