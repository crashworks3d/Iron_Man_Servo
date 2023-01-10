/*
 
MIT License

Copyright (c) 2020 Crash Works 3D

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
// Version.  Don't change unless authorized by Cranshark
#define VERSION "3.0.0.1"

#if defined __AVR_ATtiny85__ || defined __SAM3U4E__ || defined __SAM3X8E__ || defined __SAM3X8H__ || defined ARDUINO_SAMD_ZERO || defined __SAMD21G18A__  || defined __SAMD21J18A__ || ARDUINO_AVR_NANO_EVERY
  #error Code not compatible with this board type.
#endif

// Uncomment this line to enable Walsh3D MK85 Jaw Control (Open/Close)
//#define WALSH85

// Uncomment this line to enable sound for the S.U.E. expansion board
//#define SOUND    

// Uncomment this line to enable missile code
#define MISSILE

// Referenced libraries
// For installation instructions see https://github.com/netlabtoolkit/VarSpeedServo
#include <VarSpeedServo.h>

// For installation instructions see: https://github.com/fasteddy516/ButtonEvents
#include <Bounce2.h>
#include <ButtonEvents.h>

#ifdef SOUND
// See: https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299#target_6
// Important!!! On the SD card copy the mp3 files into an mp3 directory
// Download and install the DFRobotDFPlayerMini library

#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>

void printDetail(uint8_t type, int value); // header method for implementation below; affects C++ compilers
#endif

// Declare pin settings
const int servo1Pin = 9; // set the pin for servo 1
const int servo2Pin = 10; // set the pin for servo 2

#ifdef WALSH85
const int servo3Pin = 5; // set the pin for servo 3 (Walsh85 Jaw Control)
#endif

const int buttonPin = 2; // the pin that the pushbutton is attached to

// led control pins (need to be PWM enabled pins for fading)
const int leftEyePin =  6;  // left eye LEDs
const int rightEyePin = 3;  // right eye LEDs
#ifndef MISSILE
const int AuxLED = 4; // Aux LED non-PWM
#endif

#ifdef SOUND
// sound board pins
const int rx_pin = 7; // set pin for receive (RX) communications
const int tx_pin = 8; // set pin for transmit (TX) communications
#endif

// Declare servo objects
VarSpeedServo servo1; // create servo object to control servo 1
VarSpeedServo servo2; // create servo object to control servo 2

#ifdef WALSH85
VarSpeedServo servo3; // create servo object to control servo 3 (Walsh85 Jaw Control)
#endif

#ifdef MISSILE
const int servo4Pin = 4; // set the pin for servo 3 (missile bay)
const int servo5Pin = 11; // set the pin for servo 4 (missile)

const int missilePin = 12; // the pin that the missile button is attached to

VarSpeedServo servo4; // create servo object to control servo 3
VarSpeedServo servo5; // create servo object to control servo 4

// TODO: Figure out the optimal speeds
// The missile bay needs to open faster than the missile extracts
// The missile needs to retract before the missile bay closes
const int missileOpenSpeed = 200; // set the speed of the missile moving into launch position
const int missileCloseSpeed = 60; // set the speed of the missile retracting
const int missileBayOpenSpeed = 200; // set the opening speed of the missile bay 
const int missileBayCloseSpeed = 60; // set the closing speed of the missile bay

// TODO: Figure out optimal open/clos positions
const int servo4_OpenPos = 180; // set the open position of servo 4
const int servo4_ClosePos = 0; // set the closed position of servo 4
const int servo5_OpenPos = 180; //set the open position of servo 5
const int servo5_ClosePos = 0; // set the closed position of servo 5

// Define object for the missile button
ButtonEvents missileButton = ButtonEvents();

// State of the missile bay 1 = open, 0 = closed
#define MISSILE_BAY_CLOSED 0
#define MISSILE_BAY_OPEN 1
int missileBayCurMode = MISSILE_BAY_OPEN; // Keep track if the missile bay is open or closed

const int missileBayDelay = 1000; // Amount of time (ms) to delay between movement of the missile bay and the missile
#endif

// Declare variables for servo speed control
const int servoCloseSpeed = 100; // set the speed of the servo close function
const int servoOpenSpeed = 255; // set the speed of the servo opening recommend set to max speed to aid in lift

//Servo 3 (Walsh85 Jaw Control) variables for servo speed control
#ifdef WALSH85
const int jawCloseSpeed = 175; // set the speed of the Jaw closing for Walsh85 Helmet
const int jawOpenSpeed = 255; // set the speed of the Jaw opening for Walsh85 Helmet
#endif

// In Dual Servo Configuration the servos move in opposing directions, so the angles of the servos will be opposite to each other. 
// Normal Servo range is 0° ~ 180°, for initial setup the range has been adjusted to 20° ~ 160°, this allows for a 20° adjustment at both ends of the servo range.
// See Helmet tutorial for further information on servo setup.
const int servo1_OpenPos = 20; // set the open position of servo 1
const int servo2_OpenPos = 160; // set the open position of servo 2
const int servo1_ClosePos = 160; // set the closed position of servo 1
const int servo2_ClosePos = 20; // set the closed position of servo 2

#ifdef WALSH85
//Servo 3 (Walsh85 Jaw Control) Open / Close Angle
const int servo3_OpenPos = 90; // set the open position of servo 2
const int servo3_ClosePos = 0; // set the closed position of servo 1
#endif

// Declare variables for setup special effects (applies to LED eyes only for now)
#define SETUP_NONE 0 // No special effects, just turn on the LED eyes
#define SETUP_MOVIE_BLINK 1 // Blink LED eyes on setup, sequence based on Avengers Movie
#define SETUP_FADE_ON 2 // Slowly brighten LED eyes until fully lit

// To use the specific feature below
// use double slashes "//" to comment, or uncomment (remove double slashes) in the code below

// Uncomment this line if you don't want any special effect during setup, comment this line to disable this effect
// const int setupFx = SETUP_NONE;

// Uncomment this line if you want the movie blink special effect during setup, comment this line to disable this effect
const int setupFx = SETUP_MOVIE_BLINK;

// Uncomment this line if you want the fade on special effect during setup, comment this line to disable this effect
// const int setupFx = SETUP_FADE_ON;

// Declare variables for LED eyes special effects (applies to LED eyes only for now)
#define EYES_NONE 0 // No special effects, just turn on the LED eyes
#define EYES_MOVIE_BLINK 1 // Blink LED eyes on setup, sequence based on Avengers Movie
#define EYES_FADE_ON 2 // Slowly brighten LED eyes until fully lit

// To use the specific feature below
// use double slashes "//" to comment, or uncomment (remove double slashes) in the code below

// Uncomment this line if you don't want any special effect during setup, comment this line to disable this effect
// const int eyesFx = EYES_NONE;

// Uncomment this line if you want the movie blink special effect during setup, comment this line to disable this effect
// const int eyesFx = EYES_MOVIE_BLINK;

// Uncomment this line if you want the fade on special effect during setup, comment this line to disable this effect
const int eyesFx = EYES_FADE_ON;

// Declare variables for button control
boolean movieblinkOnClose = false; //Blink LEDs on close of faceplate, Sequence based on Avengers Movie

#ifndef MISSILE
// Declare variable for AuxLED
boolean auxLedEnabled = true; // Set to true if you want to enable the Aux LED
boolean auxLedState = false; // Keeps track of the state of the LED on = true, off = false
#endif

// Declare variables for LED control
unsigned long fadeDelay = .1; //speed of the eye 'fade'
unsigned long callDelay = 10; //length to wait to start eye flicker after face plate comes down
unsigned long blinkSpeed = 60; //delay between init blink on/off
unsigned long currentPWM = 0; // keep track of where the current PWM level is at
boolean isOpen = true; // keep track of whether or not the faceplate is open

#ifdef SOUND
// Declare variables for sound control
const int volume = 29; // sound board volume level (30 is max)
#define SND_CLOSE 1 // sound track for helmet closing sound
#define SND_JARVIS 2 // sound track for JARVIS sound
#define SND_OPEN 3 // sound track for helmet opening sound

SoftwareSerial serialObj(rx_pin, tx_pin); // Create object for serial communications
DFRobotDFPlayerMini mp3Obj; // Create object for DFPlayer Mini
#endif

// Define object for primary button to handle 
// multiple button press features:
// 1. Single Tap
// 2. Double Tap
// 3. Long Press
ButtonEvents primaryButton = ButtonEvents();

// State of the faceplate 1 = open, 0 = closed
#define FACEPLATE_CLOSED 0
#define FACEPLATE_OPEN 1
int facePlateCurMode = FACEPLATE_OPEN; // Keep track if the faceplate is open or closed

// State of the LED eyes 1 = on, 2 = off
#define LED_EYES_OFF 0
#define LED_EYES_ON 1

// State of the LED eyes for dimming/brightening 1 = brighten, 2 = dim
#define LED_EYES_DIM_MODE 0
#define LED_EYES_BRIGHTEN_MODE 1

int ledEyesCurMode = LED_EYES_DIM_MODE; // Keep track if we're dimming or brightening
int ledEyesCurPwm = 0; // Tracking the level of the LED eyes for dim/brighten feature
const int ledEyesIncrement = 15; // Define the increments to brighten or dim the LED eyes

/**
 * Helper Method
 * Simulate a delay in processing without disabling the processor completely
 * 
 * @param[out] period - the amount of time in milliseconds to delay
 * 
 * See: https://randomnerdtutorials.com/why-you-shouldnt-always-use-the-arduino-delay-function/
*/
void simDelay(long period){
  long delayMillis = millis() + period;
  while (millis() <= delayMillis)
  {
    int x = 0; // dummy variable, does nothing
  }
}

/**
 * Simulate the eyes slowly blinking until fully lit
 */ 
void movieblink(){
  Serial.println(F("Start Movie Blink.."));

  // pause for effect...
  simDelay(300);

  int lowValue = 21;
  int delayInterval[] = { 210, 126, 84 };
  int delayVal = 0;

  // First blink on
  for (int i = 0; i <= lowValue; i++){
    setLedEyes(i);
    setAuxLed();
    delayVal = delayInterval[0]/lowValue;
    simDelay(delayVal);
  }

  // Turn off
  setLedEyes(0);
  setAuxLed();
  simDelay(delayInterval[0]);

  // Second blink on
  for (int i = 0; i <= lowValue; i++){
    setLedEyes(i);
    setAuxLed();
    delayVal = delayInterval[1]/lowValue;
    simDelay(delayVal);
  }

  // Turn off
  setLedEyes(0);
  setAuxLed();
  simDelay(delayInterval[1]);

  // Third blink on
  setLedEyes(lowValue);
  setAuxLed();
  simDelay(delayInterval[2]);

  // Turn off
  setLedEyes(0);
  setAuxLed();
  simDelay(delayInterval[2]);

  // All on
  setLedEyes(255);
  auxLedOn();   
}

/*
 * Simulate LED eyes slowly brightening until fully lit
 */
 void fadeEyesOn(){
  ledEyesCurMode = LED_EYES_BRIGHTEN_MODE;

  // loop until fully lit
  while (ledEyesCurPwm < 255){
    setLedEyes(ledEyesCurPwm);
  
    simDelay(200);
    ledEyesBrighten();
  }
  
 }

#ifdef SOUND
/**
 * Initialization method for DFPlayer Mini board
 */
 void init_player(){
  serialObj.begin(9600);
  //simDelay(1000); Adjusting Timing Sequence

  if(!serialObj.available()){
    Serial.println(F("Serial object not available."));
  }

  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  bool dfInit = mp3Obj.begin(serialObj, false, true);

  simDelay(1000);
  
  if(!dfInit){
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));

    dfInit = mp3Obj.begin(serialObj, false, true);
    simDelay(400); // originally 1000ms
  }

  Serial.println(F("DFPlayer Mini online."));
  
  mp3Obj.setTimeOut(500); //Set serial communictaion time out 500ms
  
  Serial.println(F("Setting volume"));
  mp3Obj.volume(volume);
  simDelay(100); // DFRobot Timing 9-9-2022
  mp3Obj.EQ(DFPLAYER_EQ_NORMAL);
  mp3Obj.outputDevice(DFPLAYER_DEVICE_SD);
  simDelay(100); // DFRobot Timing 9-9-2022
 }

/**
 * Method to play the sound effect for a specified feature
 */
void playSoundEffect(int soundEffect){
  mp3Obj.volume(volume);
  simDelay(100); // DFRobot Timing 9-9-2022
  Serial.print(F("Playing sound effect: "));
  Serial.print(soundEffect);
  Serial.print(F("\tVolume: "));
  Serial.println(mp3Obj.readVolume());
  simDelay(100); // DFRobot Timing 9-9-2022
  mp3Obj.play(soundEffect);
  printDetail(mp3Obj.readType(), mp3Obj.read()); //Print the detail message from DFPlayer to handle different errors and states.
}
#endif

/**
 * Method to open face plate
 */
 void facePlateOpen(){
  Serial.println(F("Servo Up!")); 

  // Re-attach the servos to their pins
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);

  #ifdef WALSH85
  servo3.attach(servo3Pin);
  #endif

  // Send data to the servos for movement
    
  servo1.write(servo1_OpenPos, servoOpenSpeed);
  servo2.write(servo2_OpenPos, servoOpenSpeed);
  
  #ifdef WALSH85
  simDelay(500);
  servo3.write(servo3_OpenPos, jawOpenSpeed);
  //simDelay(1000); // wait doesn't wait long enough for servos to fully complete...
  #endif
  
  simDelay(1000); // wait doesn't wait long enough for servos to fully complete...

  // Detach so motors don't "idle"
  servo1.detach();
  servo2.detach();

  #ifdef WALSH85
  servo3.detach();
  #endif

  facePlateCurMode = FACEPLATE_OPEN;
 }

 /**
  * Method to close face plate
  */
 void facePlateClose(){
  Serial.println(F("Servo Down"));  

  // Re-attach the servos to their pins
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);

  #ifdef WALSH85
  servo3.attach(servo3Pin);
  #endif

  // Send data to the servos for movement 

  #ifdef WALSH85
  servo3.write(servo3_ClosePos, jawCloseSpeed);
  simDelay(500); // Delay to allow Jaw to fully close before Faceplate closes
  #endif
  
  servo1.write(servo1_ClosePos, servoCloseSpeed);
  servo2.write(servo2_ClosePos, servoCloseSpeed);

  simDelay(1000); // wait doesn't wait long enough for servos to fully complete...

  // Detach so motors don't "idle"
  servo1.detach();
  servo2.detach();

  #ifdef WALSH85
  servo3.detach();
  #endif

  facePlateCurMode = FACEPLATE_CLOSED;
 }

#ifdef MISSILE
/**
 * Method to open the missile bay
*/
 void missileBayOpen(){
  Serial.println(F("Missile bay opening..."));
  servo4.attach(servo4Pin);
  servo5.attach(servo5Pin);

  servo4.write(servo4_OpenPos, missileBayOpenSpeed);
  simDelay(missileBayDelay);
  servo5.write(servo5_OpenPos, missileOpenSpeed);

  simDelay(missileBayDelay);

  servo4.detach();
  servo5.detach();

  missileBayCurMode = MISSILE_BAY_OPEN;
 }

/**
 * Method to close the missile bay
*/
 void missileBayClose(){
  Serial.println(F("Missile bay closing..."));
  servo4.attach(servo4Pin);
  servo5.attach(servo5Pin);

  servo5.write(servo5_ClosePos, missileCloseSpeed);
  simDelay(1000);
  servo4.write(servo4_ClosePos, missileBayCloseSpeed);

  simDelay(1000);

  servo4.detach();
  servo5.detach();

  missileBayCurMode = MISSILE_BAY_CLOSED;
 }
 #endif

/**
 * Set the brightness of the LED eyes
 * 
 * @param[out] pwmValue - the PWM value (0-255) for the LED brightness
 */
void setLedEyes(int pwmValue){
  analogWrite(rightEyePin, pwmValue);
  analogWrite(leftEyePin, pwmValue);
  ledEyesCurPwm = pwmValue;
}
 
/**
 * Method to turn on LED eyes
 */
void ledEyesOn(){
  Serial.println(F("Turning LED eyes on..."));
  
  setLedEyes(255);
  
  ledEyesCurMode = LED_EYES_DIM_MODE;
}

/**
 * Method to turn off LED eyes
 */
void ledEyesOff(){
  Serial.println(F("Turning LED eyes off..."));
  
  setLedEyes(0);

  ledEyesCurMode = LED_EYES_BRIGHTEN_MODE;
}

/**
 * Method to turn LED eyes on/off
 */
void ledEyesOnOff(){
  // LED eyes stay off when faceplate is open
  if(facePlateCurMode == FACEPLATE_CLOSED){
    if (ledEyesCurPwm > 0){
      ledEyesOff();
    } else {
      ledEyesOn();
    }
  }
}

void ledEyesDim(){
  Serial.println(F("Dimming LED eyes..."));

  ledEyesCurPwm = ledEyesCurPwm - ledEyesIncrement; // Decrease the brightness

  // Make sure we don't go over the limit
  if(ledEyesCurPwm <= 0){
    ledEyesCurPwm = 0;
  }
}

void ledEyesBrighten(){
  Serial.println(F("Brightening LED eyes..."));

  ledEyesCurPwm = ledEyesCurPwm + ledEyesIncrement; // Increase the brightness

  // Make sure we don't go over the limit
  if(ledEyesCurPwm >= 255){
    ledEyesCurPwm = 255;
  }
}

/**
 * Method to dim or brighten both LED eyes
 */
void ledEyesFade(){
  if(ledEyesCurPwm == 255){
    ledEyesCurMode = LED_EYES_DIM_MODE;
  } else if(ledEyesCurPwm == 0){
    ledEyesCurMode = LED_EYES_BRIGHTEN_MODE;
  }
  
  if(ledEyesCurMode == LED_EYES_BRIGHTEN_MODE){
    ledEyesBrighten();
  } else {
    ledEyesDim();
  }

  setLedEyes(ledEyesCurPwm);

  simDelay(200);
}

/*
 * Sets the Aux LED
 */
void setAuxLed(){
#ifndef MISSILE
  if (auxLedEnabled) {
    if (auxLedState == false){
      auxLedOn();
    } else {
      auxLedOff();
    }
  } else {
    auxLedOff();
  }
#endif
}

/*
 * Turn the Aux LED on
 */
void auxLedOn(){
#ifndef MISSILE
  digitalWrite(AuxLED, HIGH);
  auxLedState = true;
#endif
}

/*
 * Turn the Aux LED off
 */
void auxLedOff(){
#ifndef MISSILE
  digitalWrite(AuxLED, LOW);
  auxLedState = false;
#endif
}

/**
 * Method to run sequence of sppecial effects when system first starts or sets up
 */
void startupFx(){
  //facePlateClose();
#ifdef MISSILE
  missileBayClose(); // Start out with the missile bay in the closed position
#endif

#ifdef SOUND
  playSoundEffect(SND_CLOSE);
  simDelay(500); // Timing for Helmet Close Sound and delay to servo closing
#endif

  facePlateClose();

  switch(setupFx){
    case SETUP_NONE:
      ledEyesOn();
      auxLedOn();
      break;
    case SETUP_MOVIE_BLINK:
      movieblink();
      break;
    case SETUP_FADE_ON:
      fadeEyesOn();
      auxLedOn();
      break;
  }

#ifdef SOUND
  simDelay(500); // Originally 800ms, changed to 500ms for DFRobot 9-9-2022
  playSoundEffect(SND_JARVIS);
#endif
}

/**
 * Method to execute special effects when the faceplate opens
 */
void facePlateOpenFx(){
  // TODO: See if we need delays in between fx
#ifdef SOUND
  playSoundEffect(SND_OPEN);
#endif

  ledEyesOff();

  facePlateOpen();
}

/**
 * Method to execute special effects when the faceplate closes
 */
void facePlateCloseFx(){
#ifdef SOUND
  playSoundEffect(SND_CLOSE);
  simDelay(1200); //Timing for Helmet Close Sound and delay to servo closing
#endif

  facePlateClose();

  switch(eyesFx){
    case EYES_NONE:
      ledEyesOn();
      auxLedOn();
      break;
    case EYES_MOVIE_BLINK:
      movieblink();
      break;
    case EYES_FADE_ON:
      fadeEyesOn();
      auxLedOn();
      break;
  }
}

/**
 * Handle faceplate special effects
 */
void facePlateFx(){
  if (facePlateCurMode == FACEPLATE_OPEN){
    facePlateCloseFx();
  } else {
    facePlateOpenFx();
  }
}

#ifdef MISSILE
void missileOpenFx(){
  missileBayOpen();
}

void missileCloseFx(){
  missileBayClose();
}

void missileFx(){
  if (missileBayCurMode == MISSILE_BAY_OPEN){
    missileCloseFx();
  } else {
    missileOpenFx();
  }
}
#endif

/**
 * Event handler for when the primary button is tapped once
 */
void handlePrimaryButtonSingleTap(){
  facePlateFx();
}

/**
 * Event handler for when the primary button is double tapped
 */
void handlePrimaryButtonDoubleTap(){
  ledEyesOnOff();
}

/**
 * Event handler for when the primary button is pressed and held
 */
void handlePrimaryButtonLongPress(){
  while(!primaryButton.update()){
    ledEyesFade(); // Dim or brighten the LED eyes
  }
}

#ifdef MISSILE
void handleMissileButtonSingleTap(){
  missileFx();
}
#endif

/**
 * Initializes the primary button for multi-functions
 */
void initPrimaryButton(){
  // Attach the button to the pin on the board
  primaryButton.attach(buttonPin, INPUT_PULLUP);
  // Initialize button features...
  primaryButton.activeLow();
  primaryButton.debounceTime(15);
  primaryButton.doubleTapTime(250);
  primaryButton.holdTime(2000);
}

#ifdef MISSILE
void initMissileButton(){
  missileButton.attach(missilePin, INPUT_PULLUP);
  missileButton.activeLow();
  missileButton.debounceTime(15);
  missileButton.doubleTapTime(250);
  missileButton.holdTime(2000);
}
#endif

/**
 * Monitor for when the primary button is pushed
 */
void monitorPrimaryButton(){
  bool changed = primaryButton.update();

  // Was the button pushed?
  if (changed){
    int event = primaryButton.event(); // Get how the button was pushed

    switch(event){
      case(tap):
        Serial.println(F("Primary button single press..."));
        handlePrimaryButtonSingleTap();
        break;
      case (doubleTap):
        Serial.println(F("Primary button double press..."));
        handlePrimaryButtonDoubleTap();
        break;
      case (hold):
        Serial.println(F("Primary button long press..."));
        handlePrimaryButtonLongPress();
        break;
    }
  }
}

#ifdef MISSILE
/**
 * Monitor for when the primary button is pushed
 */
void monitorMissileButton(){
  bool changed = missileButton.update();

  // Was the button pushed?
  if (changed){
    int event = missileButton.event(); // Get how the button was pushed

    switch(event){
      case(tap):
        Serial.println(F("Missile button single press..."));
        handleMissileButtonSingleTap();
        break;
      case (doubleTap):
        Serial.println(F("Missile button double press..."));
        break;
      case (hold):
        Serial.println(F("Missile button long press..."));
        break;
    }
  }
}
#endif

/**
 * Initialization method called by the Arduino library when the board boots up
 */
void setup() {
  // Set up serial port
  Serial.begin(115200);  
  
  simDelay(2000); // Give the serial service time to initialize

  Serial.print(F("Initializing Iron Man Servo version: "));
  Serial.println(VERSION);

#ifdef SOUND
  init_player(); // initializes the sound player
#endif

  initPrimaryButton(); // initialize the primary button
  
#ifdef MISSILE
  initMissileButton(); // initialize the missile button
#else
  pinMode(AuxLED, OUTPUT); // set output for AUX LED
#endif

  startupFx(); // Run the initial features
}

/**
 * Main program exeucution
 * This method will run perpetually on the board
 */
void loop() {
  monitorPrimaryButton(); // Since all features currently are tied to the one button...

#ifdef MISSILE
  monitorMissileButton(); // Monitor when the missile button is pushed...
#endif

  // Room for future features ;)
}

#ifdef SOUND
/**
 * Method to output any issues with the DFPlayer
 */
void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }         
      break;
    default:
      break;
  }
}
#endif
