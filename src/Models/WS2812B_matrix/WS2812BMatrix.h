#ifndef WS2812BMATRIX_H
#define WS2812BMATRIX_H

#include "Adafruit_NeoPixel.h"
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_GFX.h>

class WS2812BMatrix {
public:
    WS2812BMatrix(uint16_t width, uint16_t height, uint8_t pin, bool isRotated = false);
    void begin();
    void setBrightness(uint8_t brightness); 
    void run();
    void clear();
    void show();
    void displayScrollingText(const char* text, uint16_t color, uint32_t speed);
    void displayScrollingTextBlocking(const char* text, uint16_t color, uint32_t speed);
    void print(const char* text, uint16_t color);
    void print(const char* text, uint16_t color, int16_t x, int16_t y);
    void drawPixel(int16_t x, int16_t y, uint16_t color);

    uint16_t getWidth();
    uint16_t getHeight();

private:
    uint16_t width;
    uint16_t height;
    Adafruit_NeoMatrix matrix;
    uint16_t getPixelIndex(uint16_t x, uint16_t y);
    volatile uint64_t lastScreenClearTime;

    const char* _scrollText = nullptr;
    uint16_t _scrollColor = 0;
    int16_t _scrollX = 0;
    int16_t _scrollTextWidth = 0;
    uint32_t _scrollSpeed = 0;
    uint32_t _lastScrollUpdate = 0;
    bool _isScrolling = false;

    void _updateScrollingText();
};

#define RED                 0xF800
#define GREEN               0x07E0
#define BLUE                0x001F

#define YELLOW              0xFFE0
#define CYAN                0x07FF
#define MAGENTA             0xF81F

#define FLUSH_ORANGE        0xFC00
#define CHARTREUSE          0x87F0
#define SPRING_GREEN        0x07F8
#define AZURE               0x051F
#define ELECTRIC_INDIGO     0x801F
#define ROSE                0xF818

#define WHITE               0xFFFF
#define BLACK               0x0000

#endif // WS2812BMATRIX_H