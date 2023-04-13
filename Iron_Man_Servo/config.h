/*
 
MIT License

Copyright (c) 2023 Crash Works 3D

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

DESCRIPTION
  ====================
  The purpose of this code is to automate the servos and LED eyes for the Iron Man helmet

  Motivation and inspiration comes from the early work by "XL97" of The RPF Community

DEVELOPED BY
  ====================
  Dropwire
  Cranshark
  Taff

 */

// Uncomment this line to enable sound for the S.U.E. expansion board
//#define SOUND

// Uncomment this line to enable Walsh3D MK85 CHIN Control (Open/Close)
//#define WALSH85

// Uncomment this line to enable forearm missile special effects
//#define MISSILE

// Uncomment this line to enable a NeoPixels arc reactor
//#define ARC_REACTOR

#ifdef SOUND

#define DFPLAYER 0 
#define JQ6500 1

//#define MP3_TYPE DFPLAYER // Uncomment this line to enable using the DFRobot DFPlayer (or similar) sound module
#define MP3_TYPE JQ6500 // Uncomment this line to enable using the JQ6500 sound module

#define JARVIS 0
#define FRIDAY 1

#define SND_EFFECT_TYPE JARVIS // Uncomment this line for JARVIS sound effects
//#define SND_EFFECT_TYPE FRIDAY // Uncomment this line for FRIDAY sound effects

// sound board pins
#define RX_PIN 7 // set pin for receive (RX) communications
#define TX_PIN 8 // set pin for transmit (TX) communications

#define VOLUME 29 // sound board volume level (30 is max)
#endif

// Servo configuration types.  Use one of these values to set the SERVO_TYPE definition below
#define TPMG90S 0
#define GENERIC 1
#define MANUAL  2

// Defines which servo type is used
//#define SERVO_TYPE TPMG90S // Uncomment this line if you are using genuine Tower Pro MG90S servos
#define SERVO_TYPE GENERIC // Uncomment this line if you are using generic servos
//#define SERVO_TYPE MANUAL // Uncomment this line if you are manually configuring your servos in the manual configuration below

#if  (SERVO_TYPE == TPMG90S)
#define PWM_HIGH 2400 // Authentic Tower Pro MG90s Servo using 12% Duty Cycle
#define PWM_LOW  400 // Authentic Tower Pro MG90s Servo using 2% Duty Cycle

#elif (SERVO_TYPE == GENERIC)
#define PWM_HIGH 2600 // Generic MG90s Servo using 13% Duty Cycle
#define PWM_LOW  200 // Generic MG90s Servo using 1% Duty Cycle

// Use these settings for manual configuration of servos
#elif (SERVO_TYPE == MANUAL)
#define PWM_HIGH 2450 // Manual Setting of Duty Cycle
#define PWM_LOW  350 // Manual Setting of Duty Cycle
#endif

// Declare pin settings
#define SERVO1_PIN 9 // set the pin for servo 1
#define SERVO2_PIN 10 // set the pin for servo 2

// Declare variables for servo speed control
#define SERVO_CLOSE_SPEED 175 // set the speed of the servo close function
#define SERVO_OPEN_SPEED 255 // set the speed of the servo opening recommend set to max speed to aid in lift

// In Dual Servo Configuration the servos move in opposing directions, so the angles of the servos will be opposite to each other. 
// Normal Servo range is 0° ~ 180°, for initial setup the range has been adjusted to 20° ~ 160°, this allows for a 20° adjustment at both ends of the servo range.
// See Helmet tutorial for further information on servo setup.
#define SERVO1_OPEN_POS 20  // set the open position of servo 1
#define SERVO2_OPEN_POS 160 // set the open position of servo 2
#define SERVO1_CLOSE_POS 160 // set the closed position of servo 1
#define SERVO2_CLOSE_POS 20 // set the closed position of servo 2

// If the faceplate is dropping down when open due to centrifugal forces, change the following to false.
// Engaging a single servo may be all that is needed to keep the faceplate open
// WARNING!!! Leaving the servo(s) enabled may cause servo(s) to stress and overheat
#define SERVO1_DISABLE_AFTER_OPEN true
#define SERVO2_DISABLE_AFTER_OPEN true

#define BUTTON_PIN 2 // the pin that the pushbutton is attached to

// led control pins (need to be PWM enabled pins for fading)
#define LEFT_EYE_PIN  6 // left eye LEDs
#define RIGHT_EYE_PIN 3 // right eye LEDs

#if !defined(MISSILE) && !defined(ARC_REACTOR)
#define AUX_LED_PIN 4 // Aux LED non-PWM
#define AUX_LED_ENABLED true // Set to true if you want to enable the Aux LED
#endif

//Servo 3 (Walsh85 chin Control) variables for servo speed control
#ifdef WALSH85
#define SERVO3_PIN 5 // set the pin for servo 3 (Walsh85 chin Control)

#define CHIN_CLOSE_SPEED 180 // set the speed of the chin closing for Walsh85 Helmet
#define CHIN_OPEN_SPEED 255 // set the speed of the chin opening for Walsh85 Helmet

//Servo 3 (Walsh85 chin Control) Open / Close Angle
#define SERVO3_OPEN_POS 100 // set the open position of servo 3
#define SERVO3_CLOSE_POS 10 // set the closed position of servo 3
#endif

#ifdef MISSILE
#define SERVO4_PIN 4 // set the pin for servo 3 (missile bay)
#define SERVO5_PIN 11 // set the pin for servo 4 (missile)

#define MISSILE_BUTTON_PIN 12 // the pin that the missile button is attached to

// TODO: Figure out the optimal speeds
// The missile bay needs to open faster than the missile extracts
// The missile needs to retract before the missile bay closes
#define MISSILE_OPEN_SPEED 200 // set the speed of the missile moving into launch position
#define MISSILE_CLOSE_SPEED 60 // set the speed of the missile retracting
#define MISSILE_BAY_OPEN_SPEED 200 // set the opening speed of the missile bay 
#define MISSILE_BAY_CLOSE_SPEED 60 // set the closing speed of the missile bay

// TODO: Figure out optimal open/clos positions
#define SERVO4_OPEN_POS 180 // set the open position of servo 4
#define SERVO4_CLOSE_POS 0 // set the closed position of servo 4
#define SERVO5_OPEN_POS 180 //set the open position of servo 5
#define SERVO5_CLOSE_POS 0 // set the closed position of servo 5

#define MISSILE_BAY_DELAY 1000 // Amount of time (ms) to delay between movement of the missile bay and the missile
#endif

// Declare variables for setup special effects (applies to LED eyes only for now)

// Declare variables for LED eyes special effects (applies to LED eyes only for now)
#define EYES_NONE 0 // No special effects, just turn on the LED eyes
#define EYES_MOVIE_BLINK 1 // Blink LED eyes on setup, sequence based on Avengers Movie
#define EYES_FADE_ON 2 // Slowly brighten LED eyes until fully lit

// To use the specific feature below
// use double slashes "//" to comment, or uncomment (remove double slashes) in the code below

// Uncomment this line if you don't want any special effect during setup, comment this line to disable this effect
// #define SETUP_FX EYES_NONE

// Uncomment this line if you want the movie blink special effect during setup, comment this line to disable this effect
#define SETUP_FX EYES_MOVIE_BLINK

// Uncomment this line if you want the fade on special effect during setup, comment this line to disable this effect
// #define SETUP_FX EYES_FADE_ON

// To use the specific feature below
// use double slashes "//" to comment, or uncomment (remove double slashes) in the code below

// Uncomment this line if you don't want any special effect during setup, comment this line to disable this effect
// #define EYES_FX EYES_NONE

// Uncomment this line if you want the movie blink special effect during setup, comment this line to disable this effect
// #define EYES_FX EYES_MOVIE_BLINK

// Uncomment this line if you want the fade on special effect during setup, comment this line to disable this effect
#define EYES_FX EYES_FADE_ON

// Configuration section for the arc reactor
#ifdef ARC_REACTOR
#define PIXEL1_PIN 4 // Default pin 4
// Configures which type of pixels
// See FastLED documentation for more information.
// WARNING!!! Only clockless pixels are supported!
#define ARC_REACTOR_PIXEL_TYPE WS2812 // Default WS2812
#define ARC_REACTOR_PIXELS_NUM 7 // Number of pixels for the arc reactor
#define ARC_REACTOR_COLOR CRGB(100, 100, 255) // RGB color for the arc reactor
#define ARC_REACTOR_ALT_COLOR CRGB(0, 255, 0) // Alternate RGB color for the arc reactor
#define ARC_REACTOR_BRIGHTNESS 128 // Sets the brightness (0-255) for the arc reactor
// Pin for the button to control the arc reactor 
// -1 means no pin and the arc reactor will just turn on by default
// Possible available button pins are D12, D13, A0, A1, A2, A3, A6
#define ARC_REACTOR_BUTTON_PIN -1
#endif