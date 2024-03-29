# A.L.I.S.H.A. Board Upload Code to Arduino Nano Instructions

The purpose of this document is to provide instructions for uploading the code that runs the A.L.I.S.H.A. board. Uploading code to the Arduino Nano module is also called "flashing".

# !!! WARNING: Failure to properly follow the instructions could result in "crashing" the board and making it inoperable!!!

# !!! WARNING: Failure to unplug servos and direct power to board prior to uploading can damage the board!!! 

---
## Pre-requisites

1. Download and install Arduino IDE from [here](https://www.arduino.cc/en/software).

2. USB Mini cable capable of data transfer.

3. Crash Works 3D ALISHA board with Arduino Nano found [here](https://www.ebay.com/str/crashworks3d)

4. CH340 driver, installation instructions can be found: [How to Install CH340 Drivers](https://learn.sparkfun.com/tutorials/how-to-install-ch340-drivers/all)

## Download the code

1. Using your browser, go to the GitHub repository with the Crash Works 3D *"Iron Man Servo"*: https://github.com/crashworks3d/Iron_Man_Servo.

2. Look for the "Code" button, it should be located on the top right of the page, next to the "Go to File" button.

3. Once you click on "Code" button, you will see multiple options, select "Download ZIP"

4. Your browser will begin downloading the zip file containing the code.

5. Once the download is complete, locate the zip file on your computer and extract the contents to a folder of your choice.

### Important!!! Make sure there is a folder named "Iron_Man_Servo" and the files "Iron_Man_Servo.ino" and "config.h" are in that folder as well as a "lib" folder with files.

## Configure the code for the proper board type and features

1. Launch the Arduino IDE program by double clicking on the program icon.

2. In the Arduino IDE program, turn on verbose output by selecting from the menu one of the following:

> Windows Users: **File > Settings**

> Mac/Linux Users: **Arduino IDE > Settings**

3. In the Settings dialog window find "Show verbose output during" and check both "compile" and "upload" boxes.

4. In the Arduino IDE program the code file by selecting in the menu **File > Open...** and selecting the file `Iron_Man_Servo.ino` you downloaded in the previous steps "Download the code".

5. Select the correct board type by selecting in the menu **Tools > Board > Arduino AVR Boards > Arduino Nano**

6. Ensure the correct processor "ATmega328P" is selected by selecting in the menu **Tools > Processor > ATmega328P**

7. Verify that the code will compile by clicking on the icon with a check in it.  In the bar below the code you will see a message "Done compiling."  In the output panel you should see something like this:

>```
>Sketch uses 6006 bytes (19%) of program storage space. Maximum is 30720 bytes.
>Global variables use 375 bytes (18%) of dynamic memory, leaving 1673 bytes for local variables. Maximum is 2048 bytes.
>```

**NOTE**: If the code fails to compile go back through the instructions and make sure you didn't miss any steps.

## Upload or "Flash" the code onto the Arduino Nano module

1. Make sure you completed the previous steps ["Configure the code for the proper board type and features"](#configure-the-code-for-the-proper-board-type-and-features-configure) and that the code compiles when the "verify" button is clicked.

2. Make sure the USB cable plugged into the board.

3. Select the correct port by selecting in the menu **Tools > Port > ...**

>Windows users should see something like *"COM4"*

>Mac/Linux users should see something like *"/dev/cu.usbserial-1234"*

4. In the Arduino IDE, click on the "upload" button (The arrow icon button).

5. When the code is done uploading an "upload complete" message will pop up. In the output panel you should see something like this:

>```
>avrdude: 6006 bytes of flash written
>avrdude done.  Thank you.
>```

Successful installation will result in the servos moving the faceplate into the "closed" or "down" position and the activation of the LED (eyes).  When the button is pushed, the servos will move to the "open" or "up" position and the LED (eyes) will turn off.

NOTE: If the code fails to upload go back through the instructions and make sure you didn't miss any steps.
