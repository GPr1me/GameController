#include "AnimationLayer.h"

AnimationLayer::AnimationLayer(WS2812BMatrix& matrix)
    : matrix(matrix), currentText(nullptr), currentColor(0), animationSpeed(0),
      lastUpdateTime(0), isBlinking(false), isTextVisible(true), 
      isTextScrolling(false) {}

void AnimationLayer::begin() {
    matrix.begin();
}

void AnimationLayer::run() {
    matrix.run();

    if (isLoading) {
        updateLoading();
    }
    else if (isScoreAnimating) {
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
    isScoreAnimating = false;
    isTextScrolling = false;
    initialTime = 0;
    isLoading = false;
}

void AnimationLayer::showLoadingAnimation() {
    isLoading = true;
    lastUpdateTime = millis();
    initialTime = millis();
}

void AnimationLayer::updateLoading() {
    uint32_t startTime = millis();
    Serial.println("Loading animation started");
    
    if (startTime - lastUpdateTime >= 100) {
        int cx = matrix.getWidth() / 2;
        int cy = matrix.getHeight() / 2;
        int radius = 5; // Radius of the circle
        uint32_t frameDelay = 100; // Delay between frames in milliseconds
        const uint8_t steps = 8; // Number of points on the circle

        matrix.clear();

        // Draw the static circle (optional, for a faint outline)
        for (uint8_t i = 0; i < steps; ++i) {
            float angle = 2 * PI * i / steps;
            int x = cx + round(radius * cos(angle));
            int y = cy + round(radius * sin(angle));
            matrix.drawPixel(x, y, 0x4041); // Faint outline color
        }

        // Draw the rotating dot
        uint8_t pos = ((millis() - startTime) / frameDelay) % steps;
        float angle = 2 * PI * pos / steps;
        int x = cx + round(radius * cos(angle));
        int y = cy + round(radius * sin(angle));
        matrix.drawPixel(x, y, 0xFFE0); // Bright yellow color for the rotating dot (RGB565)

        matrix.show();
        lastUpdateTime = startTime;
    }
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

void AnimationLayer::drawMenuTitle(const uint16_t* logo, uint8_t logoWidth, uint8_t logoHeight, uint16_t bandColor, uint16_t backgroundColor, uint8_t bandWidth) {
    const uint8_t bandOffset = 3;

    // Draw band and background
    for (int x = 0; x < matrix.getWidth(); ++x) {
        for (int y = 0; y < matrix.getHeight(); ++y) {
            if (abs((x - y) - bandOffset) <= bandWidth) {
                matrix.drawPixel(x, y, bandColor);
            } else {
                matrix.drawPixel(x, y, backgroundColor);
            }
        }
    }

    // Draw logo if provided
    if (logo && logoWidth > 0 && logoHeight > 0) {
        int startX = (matrix.getWidth() - logoWidth) / 2;
        int startY = (matrix.getHeight() - logoHeight) / 2;
        for (uint8_t row = 0; row < logoHeight; ++row) {
            for (uint8_t col = 0; col < logoWidth; ++col) {
                uint16_t color = logo[row * logoWidth + col];
                // Only draw non-zero (non-transparent) pixels
                if (color != 0) {
                    matrix.drawPixel(startX + col, startY + row, color);
                }
            }
        }
    }
}

void AnimationLayer::animateMenuTitleBlinkAndGrow(const uint16_t* logo, uint8_t logoWidth, uint8_t logoHeight, uint16_t bandColor, uint16_t backgroundColor, uint8_t startWidth, uint8_t endWidth, uint16_t blinkDelay, uint8_t blinkCount) {
    for (uint8_t width = startWidth; width <= endWidth; width += 2) {
        for (uint8_t blink = 0; blink < blinkCount; ++blink) {
            // Blink: alternate between bandColor and backgroundColor
            uint16_t currentBandColor = (blink % 2 == 0) ? bandColor : backgroundColor;
            drawMenuTitle(logo, logoWidth, logoHeight, backgroundColor, width);
            matrix.show();
            delay(blinkDelay);
        }
    }
}