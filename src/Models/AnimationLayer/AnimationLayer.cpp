#include "AnimationLayer.h"

AnimationLayer::AnimationLayer(WS2812BMatrix& matrix)
    : _matrix(matrix), _currentText(nullptr), _currentColor(0), _animationSpeed(0),
      _lastUpdateTime(0), _isTextVisible(true)
    {}

void AnimationLayer::begin() {
    _matrix.begin();
}

void AnimationLayer::run() {
    switch (_currentAnimationType) {
        case SCROLLING_TEXT:
            _updateScroll();
            break;
        case BLINKING_TEXT:
            _updateBlink();
            break;
        case SCORE_ANIMATION:
            _updateScore();
            break;
        case LOADING_ANIMATION:
            _updateLoading();
            break;
    }

    _matrix.run();
}

void AnimationLayer::scrollText(const char* text, uint32_t color, uint32_t speed) {
    _currentText = text;
    _currentColor = color;
    _animationSpeed = speed;
    _lastUpdateTime = millis();
    _initialTime = millis();
    _currentAnimationType = SCROLLING_TEXT;
}

void AnimationLayer::blinkText(const char* text, uint32_t color, uint32_t interval) {
    _currentText = text;
    _currentColor = color;
    _animationSpeed = interval;
    _lastUpdateTime = millis();
    _initialTime = millis();
    _currentAnimationType = BLINKING_TEXT;
    _isTextVisible = true;
}

void AnimationLayer::showScore(int score, uint32_t color) {
    _currentScore = score;
    _currentColor = color;
    _currentAnimationType = SCORE_ANIMATION;
    _shineStep = 0;
    _lastUpdateTime = millis();
    _initialTime = millis();
}

void AnimationLayer::clear() {
    _matrix.clear();
    _currentText = nullptr;
    _initialTime = 0;
    _currentAnimationType = NONE;
}

void AnimationLayer::showLoadingAnimation() {
    _currentAnimationType = LOADING_ANIMATION;
    _lastUpdateTime = millis();
    _initialTime = millis();
}

// private methods

void AnimationLayer::_updateScroll() {
    uint32_t currentTime = millis();
    _currentAnimationType = (currentTime - _initialTime < 2000) ? SCROLLING_TEXT : NONE; // Scroll for 2 seconds
    
    if (currentTime - _lastUpdateTime >= _animationSpeed) {
        _matrix.displayScrollingText(_currentText, _currentColor, _animationSpeed);
        _lastUpdateTime = currentTime;
    }
}

void AnimationLayer::_updateBlink() {
    uint32_t currentTime = millis();
    _currentAnimationType = (currentTime - _initialTime < 2000) ? BLINKING_TEXT : NONE; // Scroll for 2 seconds
    
    if (currentTime - _lastUpdateTime >= _animationSpeed) {
        if (_isTextVisible) {
            _matrix.clear();
        } else {
            _matrix.print(_currentText, _currentColor);
        }
        _isTextVisible = !_isTextVisible;
        _lastUpdateTime = currentTime;
    }
}

void AnimationLayer::_updateScore() {
    uint32_t currentTime = millis();
    _currentAnimationType = (currentTime - _initialTime < 10000) ? SCORE_ANIMATION : NONE; // Scroll for 2 seconds
    
    if (currentTime - _lastUpdateTime >= 100) { // Adjust speed of shining effect
        char scoreText[10];
        sprintf(scoreText, "%d", _currentScore);

        // Clear the matrix before drawing
        _matrix.clear();

        // Center the score on the matrix
        int16_t x = (_matrix.getWidth() - strlen(scoreText) * 6) / 2; // 6 is the approximate width of each character
        int16_t y = (_matrix.getHeight() - 8) / 2;                   // 8 is the approximate height of characters
        _matrix.print(scoreText, _currentColor, x, y);

        // Add moving rays around the score
        for (int i = 0; i < 8; i++) { // 8 rays
            uint32_t rayColor = (i % 2 == 0) ? 0xFFFFFF : 0xFFA500; // Alternate between white and orange
            int rayX = (_matrix.getWidth() / 2) + (cos((_shineStep + i) * 0.785) * (_matrix.getWidth() / 2)); // Circular motion
            int rayY = (_matrix.getHeight() / 2) + (sin((_shineStep + i) * 0.785) * (_matrix.getHeight() / 2));
            _matrix.drawPixel(rayX, rayY, rayColor);
        }

        // Increment the shine step for animation
        _shineStep++;
        if (_shineStep > 360) { // Reset _shineStep after a full rotation
            _shineStep = 0;
        }

        _lastUpdateTime = currentTime;
    }
}

void AnimationLayer::_updateLoading() {
    uint32_t startTime = millis();
    Serial.println("Loading animation started");
    
    if (startTime - _lastUpdateTime >= 100) {
        int cx = _matrix.getWidth() / 2;
        int cy = _matrix.getHeight() / 2;
        int radius = 2; // Radius of the circle
        uint32_t frameDelay = 100; // Delay between frames in milliseconds
        const uint8_t steps = 8; // Number of points on the circle

        _matrix.clear();

        // Draw the static circle (optional, for a faint outline)
        for (uint8_t i = 0; i < steps; ++i) {
            float angle = 2 * PI * i / steps;
            int x = cx + round(radius * cos(angle));
            int y = cy + round(radius * sin(angle));
            _matrix.drawPixel(x, y, 0x4041); // Faint outline color
        }

        // Draw the rotating dot
        uint8_t pos = ((millis() - startTime) / frameDelay) % steps;
        float angle = 2 * PI * pos / steps;
        int x = cx + round(radius * cos(angle));
        int y = cy + round(radius * sin(angle));
        _matrix.drawPixel(x, y, 0xFFE0); // Bright yellow color for the rotating dot (RGB565)

        // matrix.show();
        _lastUpdateTime = startTime;
    }
}

void AnimationLayer::drawMenuTitle(const uint16_t* logo, uint8_t logoWidth, uint8_t logoHeight, uint16_t bandColor, uint16_t backgroundColor, uint8_t bandWidth) {
    const uint8_t bandOffset = 3;

    // Draw band and background
    for (int x = 0; x < _matrix.getWidth(); ++x) {
        for (int y = 0; y < _matrix.getHeight(); ++y) {
            if (abs((x - y) - bandOffset) <= bandWidth) {
                _matrix.drawPixel(x, y, bandColor);
            } else {
                _matrix.drawPixel(x, y, backgroundColor);
            }
        }
    }

    // Draw logo if provided
    if (logo && logoWidth > 0 && logoHeight > 0) {
        int startX = (_matrix.getWidth() - logoWidth) / 2;
        int startY = (_matrix.getHeight() - logoHeight) / 2;
        for (uint8_t row = 0; row < logoHeight; ++row) {
            for (uint8_t col = 0; col < logoWidth; ++col) {
                uint16_t color = logo[row * logoWidth + col];
                // Only draw non-zero (non-transparent) pixels
                if (color != 0) {
                    _matrix.drawPixel(startX + col, startY + row, color);
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
            _matrix.show();
            delay(blinkDelay);
        }
    }
}