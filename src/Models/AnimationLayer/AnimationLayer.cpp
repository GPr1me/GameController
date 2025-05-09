#include "AnimationLayer.h"

AnimationLayer::AnimationLayer(WS2812BMatrix& matrix)
    : matrix(matrix), currentText(nullptr), currentColor(0), animationSpeed(0),
      lastUpdateTime(0), isBlinking(false), isTextVisible(true), 
      isTextScrolling(false) {}

void AnimationLayer::begin() {
    matrix.begin();
}

void AnimationLayer::run() {
    uint32_t currentTime = millis();
    matrix.run();

    if (isScoreAnimating) {
        updateScore();
    } else if (isBlinking) {
        updateBlink();
    } else if (isTextScrolling) {
        updateScroll();
    }
}

void AnimationLayer::scrollText(const char* text, uint32_t color, uint32_t speed) {
    currentText = text;
    currentColor = color;
    animationSpeed = speed;
    lastUpdateTime = millis();
    initialTime = millis();
    isTextScrolling = true;
}

void AnimationLayer::blinkText(const char* text, uint32_t color, uint32_t interval) {
    currentText = text;
    currentColor = color;
    animationSpeed = interval;
    lastUpdateTime = millis();
    initialTime = millis();
    isBlinking = true;
    isTextVisible = true;
}

void AnimationLayer::showScore(int score, uint32_t color) {
    currentScore = score;
    currentColor = color;
    isScoreAnimating = true;
    shineStep = 0;
    lastUpdateTime = millis();
    initialTime = millis();
}

void AnimationLayer::clear() {
    matrix.clear();
    currentText = nullptr;
    isBlinking = false;
}

// private methods

void AnimationLayer::updateScroll() {
    uint32_t currentTime = millis();
    isTextScrolling = currentTime - initialTime < 2000; // Scroll for 2 seconds
    if (currentTime - lastUpdateTime >= animationSpeed) {
        matrix.displayScrollingText(currentText, currentColor, animationSpeed);
        lastUpdateTime = currentTime;
    }
}

void AnimationLayer::updateBlink() {
    uint32_t currentTime = millis();
    isBlinking = currentTime - initialTime < 2000; // Blink for 2 seconds
    if (currentTime - lastUpdateTime >= animationSpeed) {
        if (isTextVisible) {
            matrix.clear();
        } else {
            matrix.print(currentText, currentColor);
        }
        isTextVisible = !isTextVisible;
        lastUpdateTime = currentTime;
    }
}

void AnimationLayer::updateScore() {
    uint32_t currentTime = millis();
    isScoreAnimating = currentTime - initialTime < 10000;
    if (currentTime - lastUpdateTime >= 100) { // Adjust speed of shining effect
        char scoreText[10];
        sprintf(scoreText, "%d", currentScore);

        // Clear the matrix before drawing
        matrix.clear();

        // Center the score on the matrix
        int16_t x = (matrix.getWidth() - strlen(scoreText) * 6) / 2; // 6 is the approximate width of each character
        int16_t y = (matrix.getHeight() - 8) / 2;                   // 8 is the approximate height of characters
        matrix.print(scoreText, currentColor, x, y);

        // Add moving rays around the score
        for (int i = 0; i < 8; i++) { // 8 rays
            uint32_t rayColor = (i % 2 == 0) ? 0xFFFFFF : 0xFFA500; // Alternate between white and orange
            int rayX = (matrix.getWidth() / 2) + (cos((shineStep + i) * 0.785) * (matrix.getWidth() / 2)); // Circular motion
            int rayY = (matrix.getHeight() / 2) + (sin((shineStep + i) * 0.785) * (matrix.getHeight() / 2));
            matrix.drawPixel(rayX, rayY, rayColor);
        }

        // Increment the shine step for animation
        shineStep++;
        if (shineStep > 360) { // Reset shineStep after a full rotation
            shineStep = 0;
        }

        lastUpdateTime = currentTime;
    }
}