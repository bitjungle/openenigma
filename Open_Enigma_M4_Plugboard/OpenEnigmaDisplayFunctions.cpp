/**
 * OpenEnigmaDisplayFunctions.cpp
 * Part of Open Enigma M4 Plugboard project
 * See Open_Enigma_M4_Plugboard.ino for more information
 * 2024-12-22 - bitjungle
 */
#include "OpenEnigmaConstants.h"
#include "OpenEnigmaDisplayFunctions.h"
#include "OpenEnigmaGlobals.h"

/**
 * @brief Sends data to a 16-segment display.
 *
 * This function writes the values of four digits (dig1, dig2, dig3, dig4) to a 16-segment display
 * at positions 0, 1, 2, and 3 respectively.
 */
void nixisend() {
  sixteenSegWrite(0, dig1);
  sixteenSegWrite(1, dig2);
  sixteenSegWrite(2, dig3);
  sixteenSegWrite(3, dig4);
}

/**
 * @brief Displays a scrolling marquee text on a display.
 *
 * This function updates the display to show a scrolling marquee text.
 * It uses a static variable to keep track of the time and updates the
 * display at regular intervals. The text to be displayed is taken from
 * the MARQUEETEXT array based on the current behavior and index.
 *
 * @param None
 * @return void
 */
void marquee() {
  static unsigned long mtime;  
  time = millis();
  if ( mtime < time) {
    mtime = time + 400;
    mdex++;
  }
  dig1 = MARQUEETEXT[behavior][mdex];
  dig2 = MARQUEETEXT[behavior][mdex + 1];
  dig3 = MARQUEETEXT[behavior][mdex + 2];
  dig4 = MARQUEETEXT[behavior][mdex + 3];
  if (mdex >= MARQUEECHARS) {mdex = 0;}
  nixisend();              
}

/**
 * @brief Writes a character to a 16-segment display at a specified digit position.
 *
 * This function controls a 16-segment display by setting the appropriate segments
 * to display a given character at a specified digit position. It first activates
 * the anode for the specified digit, then sets the segment pins according to the
 * character's segment values, introduces a small delay, and finally resets the
 * segment pins and deactivates the anode.
 *
 * @param digit The digit position on the display (index in ANODEPINS array) where the character should be displayed.
 * @param character The character to be displayed (index in SEGMENTVALS array).
 */
void sixteenSegWrite(int digit, int character) {
  digitalWrite(ANODEPINS[digit],0);
  for (int index = 0; index < 17; index++) {
    digitalWrite(SEGMENTPINS[index], SEGMENTVALS[character][index]);
  }
  delay(3);
  for (int index = 0; index <= 16; index++) {
    digitalWrite(SEGMENTPINS[index], 1);   
  }
  digitalWrite(ANODEPINS[digit], 1);
}

/**
 * @brief Send DONE to the LED display.
 * 
 * Send the characters D O N E to the LED display, 
 * and reset the dig[1-4] vars to 37.
 * 
 * @param None
 * @return void
 */
void done() {
  dig1 = 3; dig2 = 14; dig3 = 13; dig4 = 4;
  nixisend();
  dig1 = 37; dig2 = 37; dig3 = 37; dig4 = 37;
}
