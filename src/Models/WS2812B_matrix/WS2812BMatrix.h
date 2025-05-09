#ifndef WS2812BMATRIX_H
#define WS2812BMATRIX_H

#include "Adafruit_NeoPixel.h"
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_GFX.h>

class WS2812BMatrix {
public:
    WS2812BMatrix(uint16_t width, uint16_t height, uint8_t pin);
    void begin();
    // void setPixelColor(uint16_t x, uint16_t y, uint32_t color);
    // void setPixelColor(uint16_t x, uint32_t color);
    void setBrightness(uint8_t brightness); 
    void run();
    void clear();
    void displayScrollingText(const char* text, uint32_t color, uint32_t speed);
    void print(const char* text, uint32_t color);
    void print(const char* text, uint32_t color, int16_t x, int16_t y);
    void drawPixel(int16_t x, int16_t y, uint32_t color);

    uint16_t getWidth();
    uint16_t getHeight();

private:
    uint16_t width;
    uint16_t height;
    // Adafruit_NeoPixel strip;
    Adafruit_NeoMatrix matrix;
    uint16_t getPixelIndex(uint16_t x, uint16_t y);
    volatile uint64_t lastScreenClearTime;
};

#endif // WS2812BMATRIX_H