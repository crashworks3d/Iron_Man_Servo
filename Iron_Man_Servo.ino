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
// Version.  Don't change unless authorized by Cranshark
#define VERSION "3.1.0.2"

#if defined __AVR_ATtiny85__ || defined __SAM3U4E__ || defined __SAM3X8E__ || defined __SAM3X8H__ || defined ARDUINO_SAMD_ZERO || defined __SAMD21G18A__  || defined __SAMD21J18A__ || ARDUINO_AVR_NANO_EVERY
  #error Code not compatible with this board type.
#endif

#include "config.h"

// Referenced libraries
// For installation instructions see https://github.com/netlabtoolkit/VarSpeedServo
#include "lib/VarSpeedServo/VarSpeedServo.cpp"

// For installation instructions see: https://github.com/mathertel/OneButton
#include "lib/OneButton/src/OneButton.cpp"

#ifdef SOUND
#ifndef MP3_TYPE
  #error MP3_TYPE not defined.  MP3_TYPE is required.
#endif

#ifndef SND_EFFECT_TYPE
  #error SND_EFFECT_TYPE not defined.  SND_EFFECT_TYPE is required.
#endif

#if (MP3_TYPE == DFPLAYER) 
// See: https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299#target_6
// Important!!! On the SD card copy the mp3 files into an mp3 directory
// Download and install the DFRobotDFPlayerMini library
#include "lib/DFRobotDFPlayerMini/DFRobotDFPlayerMini.cpp"

void printDetail(uint8_t type, int value); // header method for implementation below; affects C++ compilers
#endif

#if (MP3_TYPE == JQ6500)
// For installation instructions see: https://github.com/sleemanj/JQ6500_Serial
#include "lib/JQ6500_Serial/src/JQ6500_Serial.cpp"
#endif

#include <SoftwareSerial.h>
#endif


// Declare servo objects
VarSpeedServo servo1; // create servo object to control servo 1
VarSpeedServo servo2; // create servo object to control servo 2

#ifdef WALSH85
VarSpeedServo servo3; // create servo object to control servo 3 (Walsh85 chin Control)
#endif

#ifdef MISSILE
VarSpeedServo servo4; // create servo object to control servo 3
VarSpeedServo servo5; // create servo object to control servo 4

// Define object for the missile button
OneButton missileButton = OneButton(MISSILE_BUTTON_PIN, true, true);

// State of the missile bay 1 = open, 0 = closed
#define MISSILE_BAY_CLOSED 0
#define MISSILE_BAY_OPEN 1
int missileBayCurMode = MISSILE_BAY_OPEN; // Keep track if the missile bay is open or closed
#endif

#ifndef MISSILE
boolean auxLedState = false; // Keeps track of the state of the LED on = true, off = false
#endif

#ifdef SOUND
// Declare variables for sound control
#define SND_CLOSE 1 // sound track for helmet closing sound
#define SND_OPEN 3 // sound track for helmet opening sound
#define SND_REPULSOR 4 // sound track for repulsor sound effect
#define SND_JARVIS 2 // sound track for JARVIS sound
#define SND_FRIDAY 5 // sound track for FRIDAY sound
#define SND_NO_ACCESS 6 // sound track for "not authorized to access" sound

SoftwareSerial serialObj(RX_PIN, TX_PIN); // Create object for serial communications

#if (MP3_TYPE == DFPLAYER)
DFRobotDFPlayerMini mp3Obj; // Create object for DFPlayer Mini
#endif

#if (MP3_TYPE == JQ6500)
JQ6500_Serial mp3Obj(serialObj); // Create object for JQ6500 module
#endif
#endif

// Define object for primary button to handle 
// multiple button press features:
// 1. Single Tap
// 2. Double Tap
// 3. Long Press
OneButton primaryButton = OneButton(BUTTON_PIN, true, true);

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

#if defined (SOUND) && (MP3_TYPE == JQ6500)
#if (SND_EFFECT_TYPE == JARVIS) 
  playSoundEffect(SND_JARVIS);

  simDelay(1000);
  mp3Obj.sleep();
#else
  playSoundEffect(SND_FRIDAY);
#endif
#endif
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
#if (MP3_TYPE == DFPLAYER)
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
  mp3Obj.volume(VOLUME);
  simDelay(100); // DFRobot Timing 9-9-2022
  mp3Obj.EQ(DFPLAYER_EQ_NORMAL);
  mp3Obj.outputDevice(DFPLAYER_DEVICE_SD);
  simDelay(100); // DFRobot Timing 9-9-2022
 }

 /**
 * Method to play the sound effect for a specified feature
 */
void playSoundEffect(int soundEffect){
  mp3Obj.volume(VOLUME);
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

#if (MP3_TYPE == JQ6500)
/**
 * Initialization method for MP3 player module
 */
 void init_player(){
  serialObj.begin(9600);
  //simDelay(1000); Adjusting Timing Sequence

  if(!serialObj.available()){
    Serial.println(F("Serial object not available."));
  }

  Serial.println(F("Initializing JQ6500..."));

  mp3Obj.reset();
  mp3Obj.setSource(MP3_SRC_BUILTIN);
  mp3Obj.setVolume(VOLUME);
  mp3Obj.setLoopMode(MP3_LOOP_NONE);

  simDelay(500);
 }

/**
 * Method to play the sound effect for a specified feature
 */
void playSoundEffect(int soundEffect){
  Serial.print(F("Playing sound effect: "));
  Serial.print(soundEffect);
  mp3Obj.playFileByIndexNumber(soundEffect);
}

void delayWhilePlaying(){
  while (mp3Obj.getStatus() == MP3_STATUS_PLAYING){
    int x = 0;
  }
}
#endif
#endif

/**
 * Method to open face plate
 */
 void facePlateOpen(){
  Serial.println(F("Servo Up!")); 

  // Re-attach the servos to their pins
  servo1.attach(SERVO1_PIN, PWM_LOW, PWM_HIGH);
  servo2.attach(SERVO2_PIN, PWM_LOW, PWM_HIGH);

  #ifdef WALSH85
  servo3.attach(SERVO3_PIN, PWM_LOW, PWM_HIGH);
  #endif

  // Send data to the servos for movement
    
  servo1.write(SERVO1_OPEN_POS, SERVO_OPEN_SPEED);
  servo2.write(SERVO2_OPEN_POS, SERVO_OPEN_SPEED);
  
  #ifdef WALSH85
  simDelay(500);
  servo3.write(SERVO3_OPEN_POS, CHIN_OPEN_SPEED);
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
  servo1.attach(SERVO1_PIN, PWM_LOW, PWM_HIGH);
  servo2.attach(SERVO2_PIN, PWM_LOW, PWM_HIGH);

  #ifdef WALSH85
  servo3.attach(SERVO3_PIN, PWM_LOW, PWM_HIGH);
  #endif

  // Send data to the servos for movement 

  #ifdef WALSH85
  servo3.write(SERVO3_CLOSE_POS, CHIN_CLOSE_SPEED);
  simDelay(500); // Delay to allow chin to fully close before Faceplate closes
  #endif
  
  servo1.write(SERVO1_CLOSE_POS, SERVO_CLOSE_SPEED);
  servo2.write(SERVO2_CLOSE_POS, SERVO_CLOSE_SPEED);

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
  servo4.attach(SERVO4_PIN, PWM_LOW, PWM_HIGH);
  servo5.attach(SERVO5_PIN, PWM_LOW, PWM_HIGH);

  servo4.write(SERVO4_OPEN_POS, MISSILE_BAY_OPEN_SPEED);
  simDelay(MISSILE_BAY_DELAY);
  servo5.write(SERVO5_OPEN_POS, MISSILE_OPEN_SPEED);

  simDelay(MISSILE_BAY_DELAY);

  servo4.detach();
  servo5.detach();

  missileBayCurMode = MISSILE_BAY_OPEN;
 }

/**
 * Method to close the missile bay
*/
 void missileBayClose(){
  Serial.println(F("Missile bay closing..."));
  servo4.attach(SERVO4_PIN, PWM_LOW, PWM_HIGH);
  servo5.attach(SERVO5_PIN, PWM_LOW, PWM_HIGH);

  servo5.write(SERVO5_CLOSE_POS, MISSILE_CLOSE_SPEED);
  simDelay(1000);
  servo4.write(SERVO4_CLOSE_POS, MISSILE_BAY_CLOSE_SPEED);

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
  analogWrite(RIGHT_EYE_PIN, pwmValue);
  analogWrite(LEFT_EYE_PIN, pwmValue);
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
  if (AUX_LED_ENABLED) {
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
  digitalWrite(AUX_LED_PIN, HIGH);
  auxLedState = true;
#endif
}

/*
 * Turn the Aux LED off
 */
void auxLedOff(){
#ifndef MISSILE
  digitalWrite(AUX_LED_PIN, LOW);
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

  switch(SETUP_FX){
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

#if defined (SOUND) && (MP3_TYPE == DFPLAYER)
  simDelay(500); // Originally 2000ms
#if (SND_EFFECT_TYPE == JARVIS)
  playSoundEffect(SND_JARVIS);
#else
  playSoundEffect(SND_FRIDAY);
#endif
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

#if (MP3_TYPE == DFPLAYER)
  simDelay(1200); //Timing for Helmet Close Sound and delay to servo closing
#endif
#endif

  facePlateClose();

  switch(EYES_FX){
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
  ledEyesFade(); // Dim or brighten the LED eyes
}

#ifdef MISSILE
void handleMissileButtonSingleTap(){
  missileFx();
}
#endif

/**
 * Event handler for when the primary button is pressed multiple times
*/
void handlePrimaryButtonMultiPress(){
  switch (primaryButton.getNumberClicks())  {
    case 4:
#ifdef SOUND
      playSoundEffect(6);
#endif
      break;
    default:
      break;
  }
}

/**
 * Initializes the primary button for multi-functions
 */
void initPrimaryButton(){
  primaryButton.attachClick(handlePrimaryButtonSingleTap);
  primaryButton.attachDoubleClick(handlePrimaryButtonDoubleTap);
  primaryButton.attachDuringLongPress(handlePrimaryButtonLongPress);
  primaryButton.attachMultiClick(handlePrimaryButtonMultiPress);
}

#ifdef MISSILE
/**
 * Initializes the missile button for multi-functions
 */
void initMissileButton(){
  missileButton.attachClick(handleMissileButtonSingleTap);
}
#endif

/**
 * Monitor for when the primary button is pushed
 */
void monitorPrimaryButton(){
  primaryButton.tick();
}

#ifdef MISSILE
/**
 * Monitor for when the missile button is pushed
 */
void monitorMissileButton(){
  missileButton.tick();
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

  startupFx(); // Run the initial features

  initPrimaryButton(); // initialize the primary button
  
#ifdef MISSILE
  initMissileButton(); // initialize the missile button
#else
  pinMode(AUX_LED_PIN, OUTPUT); // set output for AUX LED
#endif
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

#if defined(SOUND) && (MP3_TYPE == DFPLAYER)
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
