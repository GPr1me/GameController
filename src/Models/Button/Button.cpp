#include "Button.h"

Button::Button(uint8_t pin, void (*callback)(Button&), uint32_t debounceDelay)
    : pin(pin), callback(callback), lastPressTime(0), debounceDelay(debounceDelay) {}

void Button::begin() {
    pinMode(pin, INPUT_PULLUP); // Configure the pin as input with pull-up resistor
}

void Button::handleToggle() {
    uint32_t currentTime = millis();
    if (currentTime - lastPressTime >= debounceDelay) {
        lastPressTime = currentTime;
        toggle = true;
    }
}

void Button::run() {
    if (toggle) {
        callback(*this);
        toggle = false;
    }
}
