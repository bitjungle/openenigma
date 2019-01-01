# Open Enigma

[![Banner](docs/banner.jpg)](https://www.instructables.com/id/Make-your-own-Enigma-Replica/)

This repository contains the Arduino code for the [S&T GeoTronics Enigma Mark 4 IP Kit Plugged](https://www.stgeotronics.com/Enigma-Mark-4-IP-Kit-Plugged-KIT-Enigma-Plugged.htm). The S&T 
GeoTronics Enigma Mark 4 contains an Arduino Mega custom shield programmed to replicate the 
behavior of a [German Enigma machine M3 and M4](https://en.wikipedia.org/wiki/Enigma_machine). 
It consists of 4 sixteen-segment display units, 5 LEDs, 26 lamps setup as keyboard, 26 keyboard 
buttons and 10 function keys. 

## Installation Instructions

Download the Arduino [source code for S&T GeoTronics Enigma Mark 4 IP Kit Plugged](Open_Enigma_M4_Plugboard/Open_Enigma_M4_Plugboard.ino). Connect your S&T GeoTronics Enigma 
Mark 4 to your computer via USB, and upload the code to the Arduino Mega using the 
[Arduino IDE](https://www.arduino.cc/en/Main/Software).

## Operation Instructions

To understand how to operate the S&T GeoTronics M4 Enigma, please refer to the following guide:

 - **[BJ Gleason - Open Enigma for Dummies](docs/BJ%20Gleason%20-%20Open%20Enigma%20for%20Dummies.pdf)**

The source code for the S&T GeoTronics Enigma Mark 4 IP Kit Plugged has been modified to 
output debug info over serial communication (via USB). It can also be set to emulate the 
Enigma M3. To switch between M4 standar, M4 debug and M3 mode, use the upper left function key.
See the source code for additional documentation.

## Release Notes

 - Designed, assembled & programmed by Marc Tessier & James Sanderson 2013-09-20
 - Modified for Prototype PCB pair on 2013-12-04
 - Modified to obey doublesteping and have M3 function 2014-03-22
 - Code formatting, cleanup and documentation + debug to serial - bitjungle 2018-12-31
