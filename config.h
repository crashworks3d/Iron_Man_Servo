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

// Uncomment this line to enable Walsh3D MK85 Jaw Control (Open/Close)
//#define WALSH85

// Uncomment this line to enable forearm missile special effects
//#define MISSILE

#ifdef SOUND
#define DFPLAYER // Uncomment this line to enable using the DFRobot DFPlayer (or similar) sound module
//#define JQ6500 // Uncomment this line to enable using the JQ6500 sound module

#define JARVIS // Uncomment this line for JARVIS sound effects
//#define FRIDAY // Uncomment this line for JARVIS sound effects

// sound board pins
#define RX_PIN 7 // set pin for receive (RX) communications
#define TX_PIN 8 // set pin for transmit (TX) communications

#define VOLUME 29 // sound board volume level (30 is max)
#endif

//#define TPMG90S
#define GENERIC
//#define MANUAL

#ifdef TPMG90S
#define PWM_HIGH 2400 // Authentic Tower Pro MG90s Servo using 12% Duty Cycle
#define PWM_LOW  400 // Authentic Tower Pro MG90s Servo using 2% Duty Cycle
#endif

#ifdef GENERIC
#define PWM_HIGH 2600 // Generic MG90s Servo using 13% Duty Cycle
#define PWM_LOW  200 // Generic MG90s Servo using 1% Duty Cycle
#endif

// Use these settings for manual configuration of servos
#ifdef MANUAL
#define PWM_HIGH 2000 // Manual Setting of Duty Cycle
#define PWM_LOW  1000 // Manual Setting of Duty Cycle
#endif

// Declare pin settings
#define SERVO1_PIN 9 // set the pin for servo 1
#define SERVO2_PIN 10 // set the pin for servo 2

// Declare variables for servo speed control
#define SERVO_CLOSE_SPEED 100 // set the speed of the servo close function
#define SERVO_OPEN_SPEED 255 // set the speed of the servo opening recommend set to max speed to aid in lift

// In Dual Servo Configuration the servos move in opposing directions, so the angles of the servos will be opposite to each other. 
// Normal Servo range is 0° ~ 180°, for initial setup the range has been adjusted to 20° ~ 160°, this allows for a 20° adjustment at both ends of the servo range.
// See Helmet tutorial for further information on servo setup.
#define SERVO1_OPEN_POS 20  // set the open position of servo 1
#define SERVO2_OPEN_POS 160 // set the open position of servo 2
#define SERVO1_CLOSE_POS 160 // set the closed position of servo 1
#define SERVO2_CLOSE_POS 20 // set the closed position of servo 2

#define BUTTON_PIN 2 // the pin that the pushbutton is attached to

// led control pins (need to be PWM enabled pins for fading)
#define LEFT_EYE_PIN  6 // left eye LEDs
#define RIGHT_EYE_PIN 3 // right eye LEDs

#ifndef MISSILE
#define AUX_LED_PIN 4 // Aux LED non-PWM
#define AUX_LED_ENABLED true // Set to true if you want to enable the Aux LED
#endif

//Servo 3 (Walsh85 Jaw Control) variables for servo speed control
#ifdef WALSH85
#define SERVO3_PIN 5 // set the pin for servo 3 (Walsh85 Jaw Control)

#define JAW_CLOSE_SPEED 175 // set the speed of the Jaw closing for Walsh85 Helmet
#define JAW_OPEN_SPEED 255 // set the speed of the Jaw opening for Walsh85 Helmet

//Servo 3 (Walsh85 Jaw Control) Open / Close Angle
#define SERVO3_OPEN_POS 90 // set the open position of servo 3
#define SERVO3_CLOSE_POS 0 // set the closed position of servo 3
#endif

#ifdef MISSILE
#define SERVO4_PIN 4 // set the pin for servo 3 (missile bay)
#define SERVO5_PIN 11 // set the pin for servo 4 (missile)

#define MISSILE_PIN 12 // the pin that the missile button is attached to

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
#define SETUP_NONE 0 // No special effects, just turn on the LED eyes
#define SETUP_MOVIE_BLINK 1 // Blink LED eyes on setup, sequence based on Avengers Movie
#define SETUP_FADE_ON 2 // Slowly brighten LED eyes until fully lit

// To use the specific feature below
// use double slashes "//" to comment, or uncomment (remove double slashes) in the code below

// Uncomment this line if you don't want any special effect during setup, comment this line to disable this effect
// #define SETUP_FX SETUP_NONE

// Uncomment this line if you want the movie blink special effect during setup, comment this line to disable this effect
#define SETUP_FX SETUP_MOVIE_BLINK

// Uncomment this line if you want the fade on special effect during setup, comment this line to disable this effect
// #define SETUP_FX SETUP_FADE_ON

// Declare variables for LED eyes special effects (applies to LED eyes only for now)
#define EYES_NONE 0 // No special effects, just turn on the LED eyes
#define EYES_MOVIE_BLINK 1 // Blink LED eyes on setup, sequence based on Avengers Movie
#define EYES_FADE_ON 2 // Slowly brighten LED eyes until fully lit

// To use the specific feature below
// use double slashes "//" to comment, or uncomment (remove double slashes) in the code below

// Uncomment this line if you don't want any special effect during setup, comment this line to disable this effect
// #define EYES_FX EYES_NONE

// Uncomment this line if you want the movie blink special effect during setup, comment this line to disable this effect
// #define EYES_FX EYES_MOVIE_BLINK

// Uncomment this line if you want the fade on special effect during setup, comment this line to disable this effect
#define EYES_FX EYES_FADE_ON

// Declare variables for button control
#define MOVIE_BLINK_ON_CLOSE false //Blink LEDs on close of faceplate, Sequence based on Avengers Movie
