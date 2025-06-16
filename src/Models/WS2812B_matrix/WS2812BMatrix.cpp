#include "WS2812BMatrix.h"
#include <cstdint>

#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_GFX.h> // Include Adafruit GFX library for text rendering

WS2812BMatrix::WS2812BMatrix(uint16_t width, uint16_t height, uint8_t pin, bool isRotated)
    : _width(width), 
    _height(height), 
    _matrix(width, height, pin, 
        isRotated 
            ? (NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE) 
            : (NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE)
    ) { }

void WS2812BMatrix::begin() {
    _matrix.begin();
    _matrix.setTextWrap(false);
}

void WS2812BMatrix::setBrightness(uint8_t brightness) {
    _matrix.setBrightness(brightness);
}

void WS2812BMatrix::run() {
    if (_isScrolling && _scrollText) {
        _updateScrollingText();
    }
    if (_lastScreenClearTime != 0) {
        if (millis() - _lastScreenClearTime >= 1000) {
            _matrix.fillScreen(0);
            _lastScreenClearTime = millis();
        }
        _matrix.show();
    }
}

void WS2812BMatrix::clear() {
    _matrix.fillScreen(0);
    _matrix.show();
    _scrollText = nullptr;
}

void WS2812BMatrix::show() {
    _matrix.show();
}

uint16_t WS2812BMatrix::_getPixelIndex(uint16_t x, uint16_t y) {
    return y * _width + x;
}

void WS2812BMatrix::displayScrollingTextBlocking(const char* text, uint16_t color, uint32_t speed) {
    _matrix.setTextColor(color);

    int16_t x = _width;
    int16_t textWidth = _matrix.width() + strlen(text) * 6; // Approximate width of text

    while (x > -textWidth) {
        _matrix.fillScreen(0);
        _matrix.setCursor(x, 0);
        _matrix.print(text);
        _matrix.show();
        delay(speed);
        x--;
    }
}

void WS2812BMatrix::displayScrollingText(const char* text, uint16_t color, uint32_t speed) {
    _scrollText = text;
    _scrollColor = color;
    _scrollSpeed = speed;
    _scrollX = _width;
    _scrollTextWidth = _matrix.width() + strlen(text) * 6; // Approximate width of text
    _lastScrollUpdate = millis();
    _isScrolling = true;
}

void WS2812BMatrix::_updateScrollingText() {
    uint32_t now = millis();
    if (now - _lastScrollUpdate >= _scrollSpeed) {
        _matrix.fillScreen(0);
        _matrix.setTextColor(_scrollColor);
        _matrix.setCursor(_scrollX, 0);
        _matrix.print(_scrollText);
        _matrix.show();

        _scrollX--;
        if (_scrollX < -_scrollTextWidth) {
            _isScrolling = false;
        }
        _lastScrollUpdate = now;
    }
}

void WS2812BMatrix::print(const char* text, uint16_t color) {
    int16_t x1, y1;
    uint16_t w, h;
    _matrix.setTextWrap(false);
    _matrix.setTextColor(color);
    _matrix.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);

    // Center the text
    int16_t cx = (_width - w) / 2;
    int16_t cy = (_height - h) / 2;

    this->print(text, color, cx, cy);
}

void WS2812BMatrix::print(const char* text, uint16_t color, int16_t x, int16_t y) {
    _matrix.setTextColor(color);
    _matrix.fillScreen(0);
    _matrix.setCursor(x, y);
    _matrix.print(text);
    _matrix.show();
    _lastScreenClearTime = millis();
}

void WS2812BMatrix::drawPixel(int16_t x, int16_t y, uint16_t color) {
    _matrix.drawPixel(x, y, color);
}


uint16_t WS2812BMatrix::getWidth() { return _width; }
uint16_t WS2812BMatrix::getHeight() { return _height; }
