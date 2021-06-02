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

  Original code created by xl97
*/
// Uncomment this line to enable sound for the S.U.E. expansion board
//#define SOUND

// Referenced libraries
// For installation instructions see https://github.com/netlabtoolkit/VarSpeedServo
#include <VarSpeedServo.h>

// For installation instructions see: https://github.com/fasteddy516/ButtonEvents
#include <Bounce2.h>
#include <ButtonEvents.h>

#ifdef SOUND
// See: https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299#target_6
// Important!!! The SD card needs to be formatted correctly as FAT32 or FAT16, and all MP3 files need to be named numerically 001 ... 002 ... 003
// Download and install the DFRobotDFPlayerMini library

#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>

void printDetail(uint8_t type, int value); // header method for implementation below; affects C++ compilers
#endif

// Declare pin settings
const int servo1Pin = 9; // set the pin for servo 1
const int servo2Pin = 10; // set the pin for servo 2
const int buttonPin = 2; // the pin that the pushbutton is attached to
// led control pins (need to be PWM enabled pins for fading)
const int leftEyePin =  6;  // left eye LEDs
const int rightEyePin =  3;  // right eye LEDs
const int AuxLED = 4; // Aux LED non-PWM
#ifdef SOUND
// sound board pins
const int rx_pin = 7; // set pin for receive (RX) communications
const int tx_pin = 8; // set pin for transmit (TX) communications
#endif

// Declare servo objects
VarSpeedServo servo1; // create servo object to control servo 1
VarSpeedServo servo2; // create servo object to control servo 2

// Declare variables for servo control
const int servoCloseSpeed = 100; // set the speed of the servo close function
const int servoOpenSpeed = 255; // set the speed of the servo opening recommend set to max speed to aid in lift
// In Dual Servo Configuration the servos move in opposing directions, so the angles of the servos will be opposite to each other. 
// Normal Servo range is 0° ~ 180°, for initial setup the range has been adjusted to 20° ~ 160°, this allows for a 20° adjustment at both ends of the servo range.
// See Helmet tutorial for further information on servo setup.
const int servo1_OpenPos = 20; // set the open position of servo 1
const int servo2_OpenPos = 160; // set the open position of servo 2
const int servo1_ClosePos = 160; // set the closed position of servo 1
const int servo2_ClosePos = 20; // set the closed position of servo 2

// Declare variables for button control
boolean movieblinkOnSetup = true; //Blink LEDs on Initial Power Up / Setup, Sequence based on Avengers Movie
boolean movieblinkOnClose = false; //Blink LEDs on close of faceplate, Sequence based on Avengers Movie

// Declare variables for LED control
unsigned long fadeDelay = .1; //speed of the eye 'fade'
unsigned long callDelay = 10; //length to wait to start eye flicker after face plate comes down
unsigned long blinkSpeed = 60; //delay between init blink on/off
unsigned long currentPWM = 0; // keep track of where the current PWM level is at
boolean isOpen = true; // keep track of whether or not the faceplate is open

#ifdef SOUND
// Declare variables for sound control
const int volume = 25; // sound board volume level (30 is max)
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
  Serial.println("Start Movie Blink..");

  // pause for effect...
  simDelay(300);

  int lowValue = 21;
  int delayInterval[] = { 210, 126, 84 };
  int delayVal = 0;

  // First blink on
  for (int i = 0; i <= lowValue; i++){
    setLedEyes(i);
    digitalWrite(AuxLED, LOW);
    delayVal = delayInterval[0]/lowValue;
    simDelay(delayVal);
  }

  // Turn off
  setLedEyes(0);
  digitalWrite(AuxLED, HIGH);
  simDelay(delayInterval[0]);

  // Second blink on
  for (int i = 0; i <= lowValue; i++){
    setLedEyes(i);
    digitalWrite(AuxLED, LOW);
    delayVal = delayInterval[1]/lowValue;
    simDelay(delayVal);
  }

  // Turn off
  setLedEyes(0);
  digitalWrite(AuxLED, HIGH);
  simDelay(delayInterval[1]);

  // Third blink on
  setLedEyes(lowValue);
  digitalWrite(AuxLED, LOW);
  simDelay(delayInterval[2]);

  // Turn off
  setLedEyes(0);
  digitalWrite(AuxLED, HIGH);
  simDelay(delayInterval[2]);

  // All on
  setLedEyes(255);
  //digitalWrite(AuxLED, LOW);   
}

#ifdef SOUND
/**
 * Initialization method for DFPlayer Mini board
 */
 void init_player(){
  serialObj.begin(9600);
  //simDelay(1000); Adjusting Timing Sequence

  if(!serialObj.available()){
    Serial.println("Serial object not available.");
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
  
  Serial.println("Setting volume");
  mp3Obj.volume(volume);
  
  mp3Obj.EQ(DFPLAYER_EQ_NORMAL);
  mp3Obj.outputDevice(DFPLAYER_DEVICE_SD);
 }

/**
 * Method to play the sound effect for a specified feature
 */
void playSoundEffect(int soundEffect){
  Serial.print("Playing sound effect: ");
  Serial.println(soundEffect);
  mp3Obj.play(soundEffect);
  printDetail(mp3Obj.readType(), mp3Obj.read()); //Print the detail message from DFPlayer to handle different errors and states.
}
#endif

/**
 * Method to open face plate
 */
 void facePlateOpen(){
  Serial.println("Servo Up!"); 

  // Re-attach the servos to their pins
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);

  // Send data to the servos for movement
  servo1.write(servo1_OpenPos, servoOpenSpeed);
  servo2.write(servo2_OpenPos, servoOpenSpeed);

  simDelay(1000); // wait doesn't wait long enough for servos to fully complete...

  // Detach so motors don't "idle"
  servo1.detach();
  servo2.detach();

  facePlateCurMode = FACEPLATE_OPEN;
 }

 /**
  * Method to close face plate
  */
 void facePlateClose(){
  Serial.println("Servo Down");  

  // Re-attach the servos to their pins
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);

  // Send data to the servos for movement 
  servo1.write(servo1_ClosePos, servoCloseSpeed);
  servo2.write(servo2_ClosePos, servoCloseSpeed);

  simDelay(1000); // wait doesn't wait long enough for servos to fully complete...

  // Detach so motors don't "idle"
  servo1.detach();
  servo2.detach();

  facePlateCurMode = FACEPLATE_CLOSED;
 }

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
  Serial.println("Turning LED eyes on...");
  
  setLedEyes(255);
  digitalWrite(AuxLED, HIGH); // Turn on Aux LED
  ledEyesCurMode = LED_EYES_DIM_MODE;
}

/**
 * Method to turn off LED eyes
 */
void ledEyesOff(){
  Serial.println("Turning LED eyes off...");
  
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
      digitalWrite(AuxLED, HIGH); // Turn on Aux LED
    }
  }
}

void ledEyesDim(){
  Serial.println("Dimming LED eyes...");

  ledEyesCurPwm = ledEyesCurPwm - ledEyesIncrement; // Decrease the brightness

  // Make sure we don't go over the limit
  if(ledEyesCurPwm <= 0){
    ledEyesCurPwm = 0;
  }
}

void ledEyesBrighten(){
  Serial.println("Brightening LED eyes...");

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

/**
 * Method to run sequence of sppecial effects when system first starts or sets up
 */
void startupFx(){
  //facePlateClose();

#ifdef SOUND
  playSoundEffect(SND_CLOSE);
  simDelay(500); // Timing for Helmet Close Sound and delay to servo closing
#endif

  facePlateClose();

  if(movieblinkOnSetup){
    movieblink();
  } else {
    ledEyesOn();
  }

#ifdef SOUND
  simDelay(800); // Originally 2000ms
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
  //facePlateClose();
  
  //simDelay(100); //Originally 1000ms

#ifdef SOUND
  playSoundEffect(SND_CLOSE);
  simDelay(1200); //Timing for Helmet Close Sound and delay to servo closing
#endif

  facePlateClose();

  if(movieblinkOnClose){
    movieblink();
  } else {
    ledEyesOn();
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
        Serial.println("Primary button single press...");
        handlePrimaryButtonSingleTap();
        break;
      case (doubleTap):
        Serial.println("Primary button double press...");
        handlePrimaryButtonDoubleTap();
        break;
      case (hold):
        Serial.println("Primary button long press...");
        handlePrimaryButtonLongPress();
        break;
    }
  }
}

/**
 * Initialization method called by the Arduino library when the board boots up
 */
void setup() {
  // Set up serial port
  Serial.begin(115200);  
  
  simDelay(2000); // Give the serial service time to initialize

#ifdef SOUND
  init_player(); // initializes the sound player
#endif

  initPrimaryButton(); // initialize the primary button
  
  pinMode(AuxLED, OUTPUT); // set output for AUX LED

  startupFx(); // Run the initial features
}

/**
 * Main program exeucution
 * This method will run perpetually on the board
 */
void loop() {
  monitorPrimaryButton(); // Since all features currently are tied to the one button...

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
