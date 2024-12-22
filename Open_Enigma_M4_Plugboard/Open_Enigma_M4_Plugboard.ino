/** S&T GeoTronics LLC  **** OPEN ENIGMA ****
 *
 *   This code is provided as is. S&T GeoTronics LLC, it's Partners and Associates 
 *   provide it freely, with no warranty of it's functionality, fitness for use or 
 *   usefulness. In no way shall S&T GeoTronics LLC, it's Partners and Associates 
 *   be liable for the use, misuse, or damages of any kind due to the use or misuse 
 *   of this code or any associated hardware provided.
 *   
 *   Enigma Code. This Arduino Mega custom shield is programmed to replicate
 *   exactly the behavior of a true German Enigma M4 machine. It uses 4 sixteen-segment 
 *   units, 5 LEDs, 26 lamps setup as keyboard, 26 keyboard buttons & 10 function 
 *   keys. The 115 light emitting diodes are multi-plexed to minimize the amount 
 *   of pins needed down to 38 and all 36 pushbuttons keys are sharing a total of 4 pins.
 * 
 *   - Designed, assembled & programmed by Marc Tessier & James Sanderson 9/20/13
 *   - Modified for our Prototype PCB pair on 12/4/13.
 *   - Modified to obey doublesteping and have M3 function 22 MAR 14
 *   - Code formatting, cleanup and documentation, serial output - bitjungle 2019-01-06
 *   - Norenigma emulation - bitjungle 2019-01-26
 *   - Moved all constants, global vars and functions to separate files, 
 *     preparing for future fixes and updates - bitjungle 2024-12-22
 *
 *   This code shall remain in public domain as regulated under the creative commons licence.
 */
#include <SoftwareSerial.h>
#include "OpenEnigmaConstants.h"
#include "OpenEnigmaGlobals.h"
#include "OpenEnigmaWheelWiring.h"
#include "OpenEnigmaModes.h"
#include "OpenEnigmaLampFunctions.h"
#include "OpenEnigmaDisplayFunctions.h"
#include "OpenEnigmaKeyboardFunctions.h"
#include "OpenEnigmaPlugboardFunctions.h"


/**
* Setting initial values for global variables
*/
unsigned long time = millis();// Number of milliseconds since start
unsigned long otime = time;   // Used in keyboard debounce code

int keyval = 100; // currently pressed key value
int kvalo = 100;  // last read key value

boolean windex = false; // windex showing true indicates the return of a fresh key stroke

int lampval = 100;
int procesval = 0;
int procesvala = 0;

int mode = 0;
int mdex = 0;

int behavior = 0; // Switch between various Enigma models 

boolean plugread = false; // Toggle after plogboard has been read

// Define each sixteen-segment display character
int dig1 = 37;
int dig2 = 37;
int dig3 = 37;
int dig4 = 37;

// Define a 2D Array for keeping the wheel locations & positions
// wheel[ ][0] : coding wheels (I-VIII is 29-34, Beta/Gamma is 1/2)
// wheel[ ][1] : inring postitions
// wheel[ ][2] : outring positions
int wheel[4][3] = {{29,0,0},
                   {28,0,0},
                   {27,0,0},
                   {1,0,0}};

//
int reflect[2] = {1,0};

// Define Array for plugbord values 25x2  
// position 0 holds use -  position 1 holds value
int plugval[2][NUMCHARS] = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25}};
int pluguse = 0; // holds the total nomber of plugs used (10 max)

/**
 * Configure Arduino pins and start serial communication
 */
void setup() {
  // Initialize all 29 LED pins as Output  
  for (int index = 0; index <= 2; index++) {
    pinMode(LAMPPINS[index], OUTPUT);
    digitalWrite(LAMPPINS[index],1);
  }
  for (int index = 0; index <= 3; index++) {
    pinMode(ANODEPINS[index], OUTPUT);
    digitalWrite(ANODEPINS[index], 1);
  }
  for (int index = 0; index <= 16; index++) {
    pinMode (SEGMENTPINS[index], OUTPUT);
    digitalWrite(SEGMENTPINS[index], 1);
  }  
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(LED4,OUTPUT);
  pinMode(LED5,OUTPUT);
  
  //Set Initial Pin Mode For Plug Board Pins
  for (int index = 0 ; index <= 25; index++) {
    pinMode(PLUGPINS[index], INPUT);
  }

  // Start serial communication
  if (DEBUG) {Serial.begin(9600);}
  
  // Initialize all 4 pushbutton pins as Input    
  for (int index = 0; index <= 3; index++) {
    pinMode(INPINS[index], INPUT);
  }
}

/**
 * Main loop
 */
void loop() {
 
  // Keyboard debounce & test for new key pressed  
  time = millis();
  if (time > otime + 100) {keyval = readkbde();}
  if (keyval == kvalo) {windex = false;} // No new keystroke detected
  kvalo = keyval;

  if ((mode == 0) && (keyval == 46) && (windex)) { 
    // Change behavior in mode 0 using key 46 (top left key)
    // 0 : Enigma M4 
    // 1 : Enigma M4 with double stepping
    // 2 : Enigma M3
    // 3 : Norenigma
    behavior++; 
    windex = false; 
    if (behavior > (EMULATORS - 1)) {
      behavior = 0; // resetting
    } 
    if (DEBUG) {Serial.print("loop() : behavior="); Serial.println(behavior);}
  }

  if ((keyval == 45) && (windex)) {modeselect();}
  // The whole Enigma machine operation revolves around which operating mode is current  
  if (mode == 0) {mode0();}
  else if (mode == 1) {mode1();} // Rotors
  else if (mode == 2) {mode2();} // Inrings
  else if (mode == 3) {mode3();} // Outrings
  else if (mode == 4) {mode4();} // Plugs
  else if (mode == 5) {mode5();} // Run
  else {mode = 0;}
}
