#include "WS2812BMatrix.h"
#include <cstdint>

#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_GFX.h> // Include Adafruit GFX library for text rendering

WS2812BMatrix::WS2812BMatrix(uint16_t width, uint16_t height, uint8_t pin)
    : width(width), 
    height(height), 
    // strip(width * height, pin, NEO_GRB + NEO_KHZ800), 
    matrix(width, height, pin, NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE) {}

void WS2812BMatrix::begin() {
    // strip.begin();
    // strip.show(); // Initialize all pixels to 'off'
    matrix.begin();
    matrix.setTextWrap(false);
}

// void WS2812BMatrix::setPixelColor(uint16_t x, uint16_t y, uint32_t color) {
//     if (x < width && y < height) {
//         strip.setPixelColor(getPixelIndex(x, y), color);
//     }
// }

// void WS2812BMatrix::setPixelColor(uint16_t x, uint32_t color) {
//     if (x < width * height) {
//         strip.setPixelColor(x, color);
//     }
// }

void WS2812BMatrix::setBrightness(uint8_t brightness) {
    matrix.setBrightness(brightness);
}

void WS2812BMatrix::run() {
    if (lastScreenClearTime != 0) {
        if (millis() - lastScreenClearTime >= 1000) {
            matrix.fillScreen(0); // Clear the matrix
            lastScreenClearTime = millis();
        }
        matrix.show();
    }
    
}

void WS2812BMatrix::clear() {
    //strip.clear();
    matrix.fillScreen(0);
    matrix.show(); // Clear
}

uint16_t WS2812BMatrix::getPixelIndex(uint16_t x, uint16_t y) {
    return y * width + x;
}

void WS2812BMatrix::displayScrollingText(const char* text, uint32_t color, uint32_t speed) {
    matrix.setTextColor(color);

    int16_t x = width;
    int16_t textWidth = matrix.width() + strlen(text) * 6; // Approximate width of text

    while (x > -textWidth) {
        matrix.fillScreen(0); // Clear the matrix
        matrix.setCursor(x, 0);
        matrix.print(text);
        matrix.show();
        delay(speed);
        x--;
    }
}

void WS2812BMatrix::print(const char* text, uint32_t color) {
    this->print(text, color, 0, 0);
}

void WS2812BMatrix::print(const char* text, uint32_t color, int16_t x, int16_t y) {
    matrix.setTextColor(color);
    matrix.fillScreen(0); // Clear the matrix
    matrix.setCursor(x, y);
    matrix.print(text);
    matrix.show();
    lastScreenClearTime = millis();
}

void WS2812BMatrix::drawPixel(int16_t x, int16_t y, uint32_t color) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        matrix.drawPixel(x, y, color);
    }
}


uint16_t WS2812BMatrix::getWidth() { return width; }
uint16_t WS2812BMatrix::getHeight() { return height; }
