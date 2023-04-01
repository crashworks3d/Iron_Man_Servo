#ifndef ARCREACTOR_H
#define ARCREACTOR_H

#include <Arduino.h>
#include "lib/OneButton/src/OneButton.h"
// TODO: Figure out how to localize...
//#include "lib/FastLED/src/FastLED.h"
#include <FastLED.h>
#include "config.h"

#ifndef ARC_REACTOR
#define PIXEL1_PIN 4 // Default pin 4
// Configures which type of pixels
// See FastLED documentation for more information.
// WARNING!!! Only clockless pixels are supported!
#define ARC_REACTOR_PIXEL_TYPE WS2812 // Default WS2812
#define ARC_REACTOR_PIXELS_NUM 1 // Number of NeoPixels for the arc reactor
#define ARC_REACTOR_COLOR CRGB(100, 100, 255) // RGB color for the arc reactor
#define ARC_REACTOR_ALT_COLOR CRGB(0, 255, 0) // Alternate RGB color for the arc reactor
#define ARC_REACTOR_BRIGHTNESS 128 // Sets the brightness (0-255) for the arc reactor
// Pin for the button to control the arc reactor 
// -1 means no pin and the arc reactor will just turn on by default
// Possible available button pins are D12, D13, A0, A1, A2, A3, A6
#define ARC_REACTOR_BUTTON_PIN -1
#endif

class ArcReactor {
public:
    // Constructor
    ArcReactor();

    // Public methods
    void initialize();
    void on();
    void off();
    void alternate();
    void handleButton();

private:
    int _numLeds;
    CRGB* _leds;
    OneButton _button;
    bool _ledsState;
    bool _ledsAltState;
    void handleSinglePress();
    static void callBackSinglePress(void *ptr);
    void handleDoublePress();
    static void callBackDoublePress(void *ptr);
};

#endif
