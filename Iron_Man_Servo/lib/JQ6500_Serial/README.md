JQ6500_Serial
=======================

Simple to use Arduino library to interface to JQ6500 (JQ6500-28P, JQ6500-16P) Mp3 Player Modules

For complete documentation about the JQ6500 Mp3 Player Module, see: 
   https://sparks.gogo.co.nz/jq6500/index.html
   
For a library methods reference see:
   https://sleemanj.github.io/JQ6500_Serial/html/index.html

For Linux Upload and Windows Upload Repair Tool (JQ6500-16) see:
   https://github.com/NikolaiRadke/JQ6500-rescue-tool

Download, Install and Example
-----------------------------

* Download: http://sparks.gogo.co.nz/JQ6500_Serial.zip
* Open the Arduino IDE (1.0.5)
* Select the menu item Sketch > Import Library > Add Library
* Choose to install the JQ6500_Serial.zip file you downloaded
* Now you can choose File > Examples > JQ6500_Serial > HelloWorld
  
Connecting To Your Arduino
--------------------------

<img src="http://sparks.gogo.co.nz/assets/_site_/images/jq6500/kq6500-16p.jpeg" align="right" title="JQ6500-16p" alt="Pinout image of JQ6500-16p MP3 Player Module For Arduino"/>
<img src="http://sparks.gogo.co.nz/assets/_site_/images/jq6500/jq6500-28.jpeg" align="right" title="JQ6500-28p" alt="Pinout image of JQ6500-28p MP3 Player Module For Arduino"/>

There are two varients of the JQ6500 module as shown.

To use this library with a *5v Arduino*, connect as follows.

| JQ6500 Module | Arduino |
| ------------- | ------- |
| RX            | through a 1K Resistor then to pin 9 |
| TX            | pin 8   |
| GND (any of)  | GND     |
| VCC (any of)  | VCC     |

To use this library with a *3v3 Arduino*, connect as follows...

| JQ6500 Module | Arduino |
| ------------- | ------- |
| RX            | pin 9   |
| TX            | pin 8   |
| GND (any of)  | GND     |
| VCC (any of)  | VCC     |

You can use pins other than 9 and 8 if you wish, simply set them in your code.

Power Demands
--------------------------

If using the on-board speaker driver, then naturally the power
demands are significant, and your USB power may not be sufficient
at more 1/3rd level of volume or so, the symptom is the audo 
breaking up and potentially resetting when volume increases.

You should use either an external power source, an external amp, or a lower
volume if you experience this problem.

Usage
--------------------------

**JQ6500 instance and serial connection**

Define a new instance of JQ6500_Serial, passing the serial connection the module is connected to.

You can use any Serial libraries that implement a *Stream* object, for example:

**Hardware** serial connection (see PlayByNumber example):

    JQ6500_Serial mp3(Serial);
    [...]
    Serial.begin(9600);

**Software** serial connection (see other examples):

    #include <SoftwareSerial.h>
    SoftwareSerial mySerial(RX_PIN, TX_PIN);
    JQ6500_Serial mp3(mySerial);
    [...]
    mySerial.begin(9600);
    

