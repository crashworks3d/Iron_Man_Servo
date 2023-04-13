#ifdef ARC_REACTOR
#include "ArcReactor.h"

// Constructor with button
ArcReactor::ArcReactor(): 
    _button(ARC_REACTOR_BUTTON_PIN, true, true), 
    _ledsState(false),
    _ledsAltState(false),
    _leds(new CRGB[ARC_REACTOR_PIXELS_NUM]){
    FastLED.addLeds<ARC_REACTOR_PIXEL_TYPE, PIXEL1_PIN, RGB>(_leds, ARC_REACTOR_PIXELS_NUM);
    FastLED.clear();

    #if (ARC_REACTOR_BUTTON_PIN != -1)
        _button.attachClick(callBackSinglePress, this);
        _button.attachDoubleClick(callBackDoublePress, this);
    #endif
}

void ArcReactor::initialize(){
    FastLED.setBrightness(ARC_REACTOR_BRIGHTNESS);
    fill_solid(_leds, ARC_REACTOR_PIXELS_NUM, CRGB::Black);
    FastLED.show();

    // Assign function to be called when button is pressed
    #if (ARC_REACTOR_BUTTON_PIN == -1)
        Serial.println("Arc Reactor button disabled...");
        on();
    #endif
}

// Turn on the ArcReactor
void ArcReactor::on() {
    _ledsState = true;
    fill_solid(_leds, ARC_REACTOR_PIXELS_NUM, ARC_REACTOR_COLOR);
    FastLED.show();
    Serial.println(F("Arc Reactor on..."));
}

// Turn off the ArcReactor
void ArcReactor::off() {
    _ledsState = false;
    fill_solid(_leds, ARC_REACTOR_PIXELS_NUM, CRGB::Black);
    FastLED.show();
    Serial.println(F("Arc Reactor off..."));
}

// Turn the ArcReactor an alternate color
void ArcReactor::alternate() {
    _ledsState = true;
    CRGB color = ARC_REACTOR_COLOR;

    if(!_ledsAltState){
        color = ARC_REACTOR_ALT_COLOR;
    }

    _ledsAltState = !_ledsAltState;
    fill_solid(_leds, ARC_REACTOR_PIXELS_NUM, color);
    FastLED.show();
}

void ArcReactor::callBackSinglePress(void *ptr) {
  ArcReactor *arcPtr = (ArcReactor *)ptr;
  arcPtr->handleSinglePress();
}

void ArcReactor::handleSinglePress() {
    if (_ledsState) {
        off();
    } else {
        on();
    }
}

void ArcReactor::callBackDoublePress(void *ptr) {
  ArcReactor *arcPtr = (ArcReactor *)ptr;
  arcPtr->handleDoublePress();
}

void ArcReactor::handleDoublePress() {
   alternate();
}

void ArcReactor::handleButton() {
    #if (ARC_REACTOR_BUTTON_PIN != -1)
        _button.tick(); // Call this method in the main loop to handle button events
     #endif
}
#endif