#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button {
public:
    Button(uint8_t pin, void (*callback)(Button&), uint32_t debounceDelay = 50);

    void begin();
    void handleToggle();
    void run();

private:
    uint8_t pin;
    void (*callback)(Button&);
    volatile uint32_t lastPressTime;
    uint32_t debounceDelay;
    bool toggle = false; // Each button has its own toggle state
};

#endif // BUTTON_H
