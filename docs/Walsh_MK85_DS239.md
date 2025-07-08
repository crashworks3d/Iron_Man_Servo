# Updating the code for the Walsh MK85 DS239

![VEK3D/Walsh3D Motorized Ironman Mark 85 Helmet 3D Printable File](../images/VEK3D-Walsh3D-MK85.jpeg)

The purpose of this guide is to provide instructions on how to update the code so that it works for the [VEK3D/Walsh3D Motorized Ironman Mark 85 Helmet 3D Printable File](https://www.wf3d.shop/collections/marvel/products/vek3d-walsh3d-motorized-ironman-mark-85-helmet-3d-printable-file)

## Pre-requisites

- See the main [README](../README.md) documentation on installing Arduino IDE, libraries and uploading to the board.
- Download the code from this repository

## Steps

- In the `config.h` file edit the following lines of code:
    - On [line 45](https://github.com/crashworks3d/Iron_Man_Servo/blob/v3.0.1.2/Iron_Man_Servo/config.h#L45):
        
        Change: `// #define WALSH85`
        
        To: `#define WALSH85`

    - On [line 79](https://github.com/crashworks3d/Iron_Man_Servo/blob/v3.0.1.2/Iron_Man_Servo/config.h#L79):

        Change: `#define SERVO_TYPE GENERIC`

        To: `// #define SERVO_TYPE GENERIC`

    - On [line 81](https://github.com/crashworks3d/Iron_Man_Servo/blob/v3.0.1.2/Iron_Man_Servo/config.h#L81):

        Change: `//#define SERVO_TYPE DS239MG`

        To: `#define SERVO_TYPE DS239MG`

- Save `config.h`
- Compile and upload

## That's it!

## Troubleshooting: 

If the code fails to compile:
- Check that you have properly downloaded **_ALL_** the files from the repository
- Check that you have **_ALL_** the required libraries properly installed
- Check that you have selected the correct board `Arduino Nano`
- Check that you properly edited the `config.h` file

If the code fails to upload:
- Check that you have a USB cable capable of data transfer
- Check that you have selected the correct port for your board
- Check that you have properly installed the CH340 drivers

For additional support, you can join a community of makers on [Frankly Built's Discord](https://discord.gg/srDqqqzj) server. Post in one of the channels like #electronics-forum and #crashworks3d-questions.