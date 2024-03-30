# Code Configuration

The purpose of this document is to provide instructions on how to configure various features of the boards.  Once you have configured your features upload your code to the board using the instructions [UPLOAD_CODE.md](UPLOAD_CODE.md)

For details on what configurations are avaialble for this version of code see our main [README.md](https://github.com/crashworks3d/Iron_Man_Servo/tree/v3.2.x?tab=readme-ov-file#feature-configurations) section.

Basically, enabling and disabling features is as simple as using 2 slashes (`//`) to comment or uncomment code.

**!!! WARNING !!!** Making changes to the configurations of your board can have unknown side effects and may cause damage. We strongly advise users to exercise caution when making changes to the board's settings, and to only do so if they have a clear understanding of the potential consequences. Improper changes to the board's configurations may result in permanent damage, loss of functionality, or other issues that could impact its performance. If you are unsure about any changes, please contact us on Frankly Built's discord: [https://discord.gg/franklybuilt](https://discord.gg/franklybuilt).

## Configure features

Using the Arduino IDE open/view the file named `config.h` to configure features such as sound, Walsh MK-85 chin servo, missile servos as well as servo positions.

### Activating Sound

To activate the sound features of the code you will need one of the Crash Works 3D boards that have either the DFPlayerMini or JQ6500 module integrated in the board such as the MK X-D or MK X-J or the external S.U.E. board.

**NOTE**: Activating sound will change the timing of the faceplate and LEDs to be in sync with the sound effects.  If you don't have a sound module and activate the sound code you may experience undesireable effects.

- Locate the line of code that has `#define SOUND`
    - Change from:
    ```C++
    //#define SOUND
    ```
    - To:
    ```C++
    #define SOUND
    ```

#### For the MK X-D or S.U.E.
- Locate the line of code that has `#define MP3_TYPE DFPLAYER`
- If the code has 2 slashes (`//`) in front of it, remove the slashes:
    - Change from:
    ```C++
    //#define MP3_TYPE DFPLAYER
    ```
    - To:
    ```C++
    #define MP3_TYPE DFPLAYER
    ```
- If the line below it **_DOES NOT_** have 2 slashes in front of it, put 2 slashes in front of it:
    - Change from:
    ```C++
    //#define MP3_TYPE JQ6500
    ```
    - To:
    ```C++
    #define MP3_TYPE JQ6500
    ```

#### For the MK X-J
- Locate the line of code that has `#define MP3_TYPE DFPLAYER`
- If the code **_DOES NOT_** have 2 slashes (`//`) in front of it, put 2 slashes in front of it:
    - Change from:
    ```C++
    #define MP3_TYPE DFPLAYER
    ```
    - To:
    ```C++
    //#define MP3_TYPE DFPLAYER
    ```
- If the line below it has 2 slashes in front of it, remove the 2 slashes in front of it:
    - Change from:
    ```C++
    //#define MP3_TYPE JQ6500
    ```
    - To:
    ```C++
    #define MP3_TYPE JQ6500
    ```
#### Adjusting volume
- In the `config.h` file, locate the line of code `#define VOLUME 27`
- Change the number to your desired volume level where `30` is the maximum value.

#### Changing from _JARVIS_ to _FRIDAY_

You can change the sound effects to use either the voice of _JARVIS_ or _FRIDAY_ using the steps below.

- If you have an A.L.I.S.H.A. board with an SD card like the MK X-D:
    - In the [mp3](https://github.com/crashworks3d/Iron_Man_Servo/tree/v3.2.x/mp3) folder of this repository download the file `0005.mp3`
    - Rename the file from `0005.mp3` to `0002.mp3`
    - Copy the file `0002.mp3` to your SD card overwriting the existing file.  (NOTE: you may need to reload all the mp3 files to your SD card in order as described in the [SD card instructions](https://github.com/crashworks3d/Iron_Man_Servo/tree/v3.2.x/instructions/Crash%20Works%203D%20-%20SD%20Card%20Formatting%20Instructions.docx.pdf))

- If you have an ALISHA board with the JQ6500 sound module like the MK X-J:
    - In the `config.h` file locate the lines of code below and make the following changes:
    
    - Change from:
    ```C++
    #define SND_EFFECT_TYPE JARVIS // Uncomment this line for JARVIS sound effects
    //#define SND_EFFECT_TYPE FRIDAY // Uncomment this line for FRIDAY sound effects
    ```

    - To:
    ```C++
    //#define SND_EFFECT_TYPE JARVIS // Uncomment this line for JARVIS sound effects
    #define SND_EFFECT_TYPE FRIDAY // Uncomment this line for FRIDAY sound effects
    ```

### Adjusting Servos

After thorough testing, we've determined that the following servo degree settings are optimal for the majority of faceplate articulations.  Before making changes in code, make sure you have properly installed your servo mount, helper arms and faceplate mount in your helmet.

**_WARNING!!!_** Making adjustments in code to your servo settings could damage your helmet, board and/or servos and may cause unpredictable servo movements.  Read the specifications related to your servos prior to making any changes.

#### Servo positions

To adjust servo positions locate the following code and change the values in number of degrees:

```C++
#define SERVO1_OPEN_POS 20  // set the open position of servo 1
#define SERVO2_OPEN_POS 160 // set the open position of servo 2
#define SERVO1_CLOSE_POS 160 // set the closed position of servo 1
#define SERVO2_CLOSE_POS 20 // set the closed position of servo 2
```

#### Servo speed

To adjust servo speed locate the following code and change the values where the maximum value is 255:

```C++
#define SERVO_CLOSE_SPEED 175 // set the speed of the servo close function
#define SERVO_OPEN_SPEED 255 // set the speed of the servo opening recommend set to max speed to aid in lift
```

#### Disable after open

**_WARNING!!!_** By design, the servos are disabled after opening in order to prevent overheating and power drain.

To adjust disabling the servos when the faceplate is in the open position locate the following code and change the value from `true` to `false`:

```C++
#define SERVO1_DISABLE_AFTER_OPEN true
#define SERVO2_DISABLE_AFTER_OPEN true
```

#### PWM values

**_WARNING!!!_** Don't change these values unless you really understand **_Pulse-Width-Modulation_** and your servo specifications.

Locate the following code and change values as needed:
```C++
// Use these settings for manual configuration of servos
#elif (SERVO_TYPE == MANUAL)
#define PWM_HIGH 2500 // Manual Setting of Duty Cycle
#define PWM_LOW  300 // Manual Setting of Duty Cycle
#endif
```

### Activating Pixels or NeoPixels

The current code in this repository is designed to light up an arc reactor using an 8x8 NeoPixels or WS2812 panel.  See the [Adafruit NeoPixel Überguide](https://learn.adafruit.com/adafruit-neopixel-uberguide).

The code was developed using the [FastLED](https://fastled.io/) library.  You will want to reference this library if you would like to extend the code for features like Repulsors.

- In the `config.h` file, locate the line of code that has `#define ARC_REACTOR` and make the following changes:
    
    From:
    ```C++
    // Uncomment this line to enable a NeoPixels arc reactor
    //#define ARC_REACTOR
    ```

    To:
    ```C++
    // Uncomment this line to enable a NeoPixels arc reactor
    #define ARC_REACTOR
    ```

- To make changes to the number of LEDs, color or brightness; In the `config.h` file, locate the following code and change the values:

```C++
// Configuration section for the arc reactor
#ifdef ARC_REACTOR
#define PIXEL1_PIN 4 // Default pin 4
// Configures which type of pixels
// See FastLED documentation for more information.
// WARNING!!! Only clockless pixels are supported!
#define ARC_REACTOR_PIXEL_TYPE WS2812 // Default WS2812
#define ARC_REACTOR_PIXELS_NUM 64 // Number of pixels for the arc reactor
#define ARC_REACTOR_COLOR CRGB(100, 100, 255) // RGB color for the arc reactor
#define ARC_REACTOR_ALT_COLOR CRGB(0, 255, 0) // Alternate RGB color for the arc reactor
#define ARC_REACTOR_BRIGHTNESS 128 // Sets the brightness (0-255) for the arc reactor
// Pin for the button to control the arc reactor 
// -1 means no pin and the arc reactor will just turn on by default
// Possible available button pins are D12, D13, A0, A1, A2, A3, A6
#define ARC_REACTOR_BUTTON_PIN -1
#endif
```