#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include <functional>

class Button {
public:
    Button(uint8_t pin, std::function<void(Button&)>, uint32_t debounceDelay = 100);

    void begin();
    void handleToggle();
    void run();
    void setCallback(std::function<void(Button&)> callback);
    void setDebounceDelay(uint32_t delay);
    void disableInterrupts();

    bool isPressed() const;

private:
    uint8_t _pin;
    std::function<void(Button&)> _callback;
    volatile uint32_t _lastPressTime;
    uint32_t _debounceDelay;
    bool _toggle;
    bool _isInterruptEnabled;
};

#endif // BUTTON_H
