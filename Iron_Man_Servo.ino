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

// Referenced libraries
// For installation instructions see https://github.com/netlabtoolkit/VarSpeedServo
#include <VarSpeedServo.h>

// Declare pin settings
const int servo1Pin = 9; // set the pin for servo 1
const int servo2Pin = 10; // set the pin for servo 2
const int buttonPin = 2; // the pin that the pushbutton is attached to
// led control pins (need to be PWM enabled pins for fading)
const int leftEyePin =  6;  // left eye LEDs
const int rightEyePin =  3;  // right eye LEDs
const int AuxLED = 4; // Aux LED non-PWM

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
int buttonState = 0; // current state of the button
int lastButtonState = 0; // previous state of the button
boolean movieblinkOnSetup = true; //Blink LEDs on setup, Sequence based on Avengers Movie
boolean movieblinkOnClose = false; //Blink LEDs on close of faceplate, Sequence based on Avengers Movie

// Declare variables for LED control
unsigned long fadeDelay = .1; //speed of the eye 'fade'
unsigned long callDelay = 10; //length to wait to start eye flicker after face plate comes down
unsigned long blinkSpeed = 60; //delay between init blink on/off
unsigned long currentPWM = 0; // keep track of where the current PWM level is at
boolean isOpen = true; // keep track of whether or not the faceplate is open

#define S_IDLE 1
#define S_LEDON 2
#define S_WAITON 3
#define S_LEDOFF 4
#define S_WAITOFF 5
#define S_INITON 6
#define S_INITWAIT 7
#define S_MOVIEBLINK 8
#define S_SERVOUP 9
#define S_SERVODOWN 0
#define S_SERVOWAIT 10



//FSM init vars
static int state = S_LEDON; // initial state is 1, the "idle" state.
static unsigned long lastTime;  // To store the "current" time in for delays.

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
    analogWrite(rightEyePin, i);
    analogWrite(leftEyePin, i);
    digitalWrite(AuxLED, LOW);
    delayVal = delayInterval[0]/lowValue;
    simDelay(delayVal);
  }

  // Turn off
  analogWrite(rightEyePin, 0);
  analogWrite(leftEyePin, 0);
  digitalWrite(AuxLED, HIGH);
  simDelay(delayInterval[0]);

  // Second blink on
  for (int i = 0; i <= lowValue; i++){
    analogWrite(rightEyePin, i);
    analogWrite(leftEyePin, i);
    digitalWrite(AuxLED, LOW);
    delayVal = delayInterval[1]/lowValue;
    simDelay(delayVal);
  }

  // Turn off
  analogWrite(rightEyePin, 0);
  analogWrite(leftEyePin, 0);
  digitalWrite(AuxLED, HIGH);
  simDelay(delayInterval[1]);

  // Third blink on
  analogWrite(rightEyePin, lowValue);
  analogWrite(leftEyePin, lowValue);
  digitalWrite(AuxLED, LOW);
  simDelay(delayInterval[2]);

  // Turn off
  analogWrite(rightEyePin, 0);
  analogWrite(leftEyePin, 0);
  digitalWrite(AuxLED, HIGH);
  simDelay(delayInterval[2]);

  // All on
  analogWrite(rightEyePin, 255);
  analogWrite(leftEyePin, 255);
  digitalWrite(AuxLED, LOW);
  
  state = S_LEDON;    
}
/**
 * Initialization method called by the Arduino library when the board boots up
 */
void setup() {
  // Set up serial port
  Serial.begin(9600);  
  Serial.print("Initial State: ");
  Serial.println(state);

  servo1.attach(servo1Pin); // attaches the servo on pin 9 to the servo object
  servo2.attach(servo2Pin); // attaches the 2nd servo on pin 10 to the servo object

  servo1.write(servo1_ClosePos, servoCloseSpeed); // sets intial position for 1st servo
  servo2.write(servo2_ClosePos, servoCloseSpeed); // sets intial position for 2nd servo

  simDelay(1000); // wait for servos to complete range of motion

  // Detach so motors don't "idle"
    servo1.detach();
    servo2.detach();  

  pinMode(buttonPin, INPUT); // initialize the button pin as a input
  digitalWrite(buttonPin, HIGH); //use interal pull up resistors
  pinMode(AuxLED, OUTPUT); // set output for AUX LED
  
  //start with blinking LEDs if flag is turned on
  if( movieblinkOnSetup )
  {
    Serial.print("Blink on Setup Running");
    movieblink(); // Call the method to simulate Avengers Movie Blink Sequence
  }
}
void loop() { 
  
  switch(state)
  {
  case S_IDLE:
    // We don't need to do anything here, waiting for a forced state change, like a button press.
    //check main button state
    buttonState = digitalRead(buttonPin);

    // compare buttonState to previous state  
    if (buttonState != lastButtonState) {
      //if button pressed/down
      if (buttonState == LOW){
        //ie: pressed   
        if(isOpen == true){
          Serial.print("CLOSING FACE PLATE: ");
          Serial.println(isOpen, DEC); 
          state = S_SERVODOWN;
        }
        else{
          Serial.print("OPENING FACE PLATE: ");
          Serial.println(isOpen, DEC); 
          state = S_SERVOUP;
          state = S_LEDOFF;
        }
        isOpen = !isOpen;
      } 
      else{
        //went from ON/HIGH to LOW/OFF..ie: released
        //Serial.print("RELEASE: ");
        //Serial.println(isOpen, DEC); 
      } 
    }
    //save the current state for next loop
    lastButtonState = buttonState;
    break;

  case S_MOVIEBLINK:
    Serial.println("Movie Blink State");   
    movieblink(); // Call the method to simulate the eyes blinking on
    break;

  case S_LEDON:
    //Serial.println("Increase");    
    lastTime = millis();  // Remember the current time
    analogWrite(leftEyePin, currentPWM); // Turn on Left LED
    analogWrite(rightEyePin, currentPWM); // Turn on Right LED
    digitalWrite(AuxLED, HIGH); // Turn on AUX LED
    state = S_WAITON;  // Move to the next state
    break;

  case S_WAITON:
    // If one second has passed, then move on to the next state.
    if(millis() > (lastTime + fadeDelay)){
      if(currentPWM < 255){
        currentPWM += 5;
        state = S_LEDON;        
      }
      else{
        Serial.println("@ 255 done");
        state = S_IDLE;
        //state = S_LEDOFF; //no auto turn off.. set to idle state
      }
    }
    break;

  case S_LEDOFF:
    //Serial.println("........decrease");     
    lastTime = millis();  // Remember the current time
    analogWrite(leftEyePin, currentPWM); // Turn off Left LED
    analogWrite(rightEyePin, currentPWM); // Turn off Right LED
    digitalWrite(AuxLED, LOW); // Turn off AUX LED
    state = S_WAITOFF;
    break;

  case S_WAITOFF:
    // If one second has passed, then move on to the next state.
    if(millis() > (lastTime + fadeDelay)){
      if(currentPWM > 0){  //change 0 to higher number to init face 'up' function sooner.
        currentPWM -= 5;
        state = S_LEDOFF;        
      }
      else{
        Serial.println("Done");
        state = S_SERVOUP; //leds off..raise faceplate
      }
    }
    break; 

  case S_SERVOUP:
    lastTime = millis();  // Remember the current time
    Serial.println("Servo Up"); 

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

    state = S_IDLE;    
    break;

  case S_SERVODOWN:
    lastTime = millis();  // Remember the current time
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

    if( movieblinkOnClose ) {
      Serial.println("Blink on Close Running");
      movieblink(); // Call the method to simulate Avengers Movie Blink Sequence
    }
    state = S_LEDON;    
    break;

  case S_SERVOWAIT:
    // If enough time has passed, call the eye flicker routine
    if(millis() > (lastTime + callDelay)){   
      Serial.println("start eye routine");
      state = S_LEDON;        
    }
    else{
      Serial.println("Waiting");
    }
    break;

  default:
    state = S_IDLE;
    break;
  } 
}
