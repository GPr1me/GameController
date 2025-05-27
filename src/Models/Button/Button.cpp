#include "Button.h"

Button::Button(uint8_t pin, std::function<void(Button&)> callback, uint32_t debounceDelay)
    : _pin(pin), _callback(callback), _lastPressTime(0), _debounceDelay(debounceDelay) 
{
    _toggle = false;
    _isInterruptEnabled = true;
}

void Button::begin() {
    pinMode(_pin, INPUT_PULLUP); // Configure the pin as input with pull-up resistor
}

void Button::handleToggle() {
    uint32_t currentTime = millis();
    if (currentTime - _lastPressTime >= _debounceDelay) {
        _lastPressTime = currentTime;
        _toggle = true;
    }
}

void Button::run() {
    if (_toggle || (!_isInterruptEnabled && isPressed())) {
        if (_callback) _callback(*this);
        _toggle = false;
    }
}

void Button::setCallback(std::function<void(Button&)> callback) {
    this->_callback = callback;
}

void Button::setDebounceDelay(uint32_t delay) {
    _debounceDelay = delay;
}

bool Button::isPressed() const {
    return digitalRead(_pin) == LOW;
}

void Button::disableInterrupts() {
    detachInterrupt(digitalPinToInterrupt(_pin));
    _isInterruptEnabled = false;
}