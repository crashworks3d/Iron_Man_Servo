# Iron Man Helmet Servo Controller for Arduino Nano

![Iron Man Mark 85 Avengers End Game - Marvel](images/DSC01481.png)

The purpose of this project is to automate features of the various models of the Iron Man 3D printed helmet.  This project was created with the purpose of enabling fans like you to be able to more easily make your own automated helmet. We encourage you to learn from this as well as provide feedback for us to improve upon it.

## Follow Me

[![alt text][1.1]][1]  [![alt text][2.1]][2]  [![alt text][3.1]][3]   [![alt text][4.1]][4]

[1.1]: images/instagram.png (Instagram - Dropwire)
[2.1]: images/youtube.png (YouTube - Dropwire)
[3.1]: images/github.png (Github - Crash Works)
[4.1]: images/Thingiverse.png (Thingiverse - Crash Works)
[5.1]: images/youtube-preview.jpg (Crash Works 3d - MK85 Helmet Motorization Kit Test v1)
[6.1]: images/youtube-preview_MK7.JPG (Crash Works 3d - MK7 Helmet Motorization Kit Test v1)
[7.1]: images/youtube-preview_Mk46_Closed.JPG (Crash Works 3d - MK46 Helmet Motorization Kit Test v1)
[1]: https://www.instagram.com/dropwire/
[2]: https://www.youtube.com/channel/UCxbZNWPNsDoVIHJfYZQF2Jw
[3]: https://github.com/
[4]: https://www.thingiverse.com/crashworks3d/designs
[5]: https://www.youtube.com/watch?v=dDsZCZuzkQU
[6]: https://www.youtube.com/watch?v=P-p2FFcd4s8&feature=emb_logo
[7]: https://www.youtube.com/watch?v=9Ue6Bggzcgk&feature=emb_logo

### Actual Use:

[![alt text][5.1]][5]
[![alt text][6.1]][6]
[![alt text][7.1]][7]

## Getting Started

It will be helpful but not necessary if you have a basic level of programming experience as well as a little experience programming Arduino micro controllers.  This project was created using the [Aurduino IDE](https://www.arduino.cc/).  Clone or download this repository at your own risk.

You can download the 3D printable servo kit from Thingiverse:

[Thingiverse MK85 Iron Man Servo Kit](https://www.thingiverse.com/thing:4607836)

[Thingiverse MK7 Iron Man Servo Kit](https://www.thingiverse.com/thing:4630066)

[Thingiverse MK46 Iron Man Servo Kit](https://www.thingiverse.com/thing:4640029)

**You can purchase an easy to install and use Arduino shield (Nano included) designed specifically for this project:**

**_A.L.I.S.H.A._ - Arduino nano Led & Integrated Servo Helmet Actuator for 3D Printed Helmets**

[eBay - A.L.I.S.H.A. SMT](https://www.ebay.com/itm/402549198649)  

[![alt text](images/SMT.jpg)](https://www.ebay.com/itm/402549198649)

[eBay - A.L.I.S.H.A. Through-Hole](https://www.ebay.com/itm/402549215661)

[![alt text](images/THB.jpg)](https://www.ebay.com/itm/402549215661)

### Prerequisites

* Some experience with [Arduino](https://www.arduino.cc/)

* Be sure to have the Arduino Nano board installed in your environment.
* Install the [VarSpeedServo](https://github.com/netlabtoolkit/VarSpeedServo) library. 
* An Arduino Nano board
* Servos
* LEDs
* Resistors

(See BOM below for details.)

### Installing

1.  In the Arduino IDE be sure to set the appropriate board (Arduino Nano) and ports to match your environment.
2. Complile and load onto your Arduino Nano following the [Arduino IDE instructions](https://www.arduino.cc/en/Guide).

Successful installation will result in the servos moving the faceplate into the "closed" or "down" position and the activation of the LED (eyes).  When the button is pushed, the servos will move to the "open" or "up" position and the LED (eyes) will turn off.

## Wiring Diagram
![Wiring diagram](images/wiring_diagram_1.jpg)

## Breadboard Example
![Breadboard example](images/Iron_Man_Servo_Sketch_bb.jpg)

## BOM
Most components can be found on [Amazon](https://www.amazon.com) or [AliExpress](https://www.aliexpress.com)
* Arduino Nano
* MG90S Servo (metal gear only!)
* Cosplay Iron Man/Batman LEDs (can be found on eBay or AliExpress)
* 100 ohm resistors x2 (1 for each LED)

## Built With

* [Arduino](https://www.arduino.cc/)

## Versioning

Version Beta 1.0 (Use at your own risk) 

## Authors

* **xl97** - *Initial work*

* **Dropwire**

* **Cranshark @ i3 Creations**

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments/ Hat Tip
* [XL97](https://www.therpf.com/forums/members/xl97.9819/)
* [i3 Creations, LLC](https://github.com/i3creations)
* [Dropwire](https://github.com/Acollazo7)
* [Arduino](https://www.arduino.cc/)
* [Fritzing](https://fritzing.org/)
