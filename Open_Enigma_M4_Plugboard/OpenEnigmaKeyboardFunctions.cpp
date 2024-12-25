/**
 * OpenEnigmaKeyboardFunctions.cpp
 * Part of Open Enigma M4 Plugboard project
 * See Open_Enigma_M4_Plugboard.ino for more information
 * 2024-12-22 - bitjungle
 */
#include "OpenEnigmaConstants.h"
#include "OpenEnigmaGlobals.h"
#include "OpenEnigmaKeyboardFunctions.h"

/**
 * @brief Reads the keyboard input values and determines the key pressed.
 * 
 * This function reads analog input values from the keyboard, processes them,
 * and returns a corresponding key value. It also handles key debounce timing
 * and optionally prints debug information to the serial monitor.
 * 
 * @return int The key value corresponding to the key pressed. Returns 100 if no key is pressed.
 */
int readkbde() {
  int inval[4] = {0, 0, 0, 0};
  int kval = 100;

  for (int index = 0; index <= 3; index++) { //Read analog input values
    inval[index] = analogRead(INPINS[index]);   
   } 

  if ((inval[0] > 982) && (inval[1] > 973) && (inval[2] > 966) && (inval[3] > 973))  {kval = 100;} // no key press
  else if ((inval[0] < 981) && (inval[0] > 903)) {kval = 49;} //up arrow 4
  else if ((inval[0] < 902) && (inval[0] > 831)) {kval = 48;} //up arrow 3
  else if ((inval[0] < 830) && (inval[0] > 762)) {kval = 47;} //up arrow 2
  else if ((inval[0] < 761) && (inval[0] > 693)) {kval = 46;} //up arrow 1
  else if ((inval[0] < 692) && (inval[0] > 622)) {kval = 45;} //mode
  else if ((inval[0] < 621) && (inval[0] > 545)) {kval = 44;} //enter
  else if ((inval[0] < 544) && (inval[0] > 458)) {kval = 43;}
  else if ((inval[0] < 457) && (inval[0] > 358)) {kval = 42;}
  else if ((inval[0] < 357) && (inval[0] > 237)) {kval = 41;}
  else if (inval[0] < 236 ) {kval = 40;}
  else if ((inval[1] < 972) && (inval[1] > 875)) {kval = 16;}
  else if ((inval[1] < 874) && (inval[1] > 784)) {kval = 22;}
  else if ((inval[1] < 783) && (inval[1] > 696)) {kval = 4;}
  else if ((inval[1] < 695) && (inval[1] > 605)) {kval = 17;}
  else if ((inval[1] < 604) && (inval[1] > 507)) {kval = 19;}
  else if ((inval[1] < 506) && (inval[1] > 395)) {kval = 25;}
  else if ((inval[1] < 394) && (inval[1] > 261)) {kval = 20;}
  else if ((inval[1] < 260) && (inval[1] > 94)) {kval = 8;}
  else if (inval[1] < 93 ) {kval = 14;}
  else if ((inval[2] < 965) && (inval[2] > 855)) {kval = 0;}
  else if ((inval[2] < 854) && (inval[2] > 752)) {kval = 18;}
  else if ((inval[2] < 751) && (inval[2] > 650)) {kval = 3;}
  else if ((inval[2] < 649) && (inval[2] > 542)) {kval = 5;}
  else if ((inval[2] < 541) && (inval[2] > 421)) {kval = 6;}
  else if ((inval[2] < 420) && (inval[2] > 278)) {kval = 7;}
  else if ((inval[2] < 277) && (inval[2] > 100)) {kval = 9;}
  else if (inval[2] < 99 ) {kval = 10;}
  else if ((inval[3] < 972) && (inval[3] > 875)) {kval = 15;}
  else if ((inval[3] < 874) && (inval[3] > 784)) {kval = 24;}
  else if ((inval[3] < 783) && (inval[3] > 696)) {kval = 23;}
  else if ((inval[3] < 695) && (inval[3] > 605)) {kval = 2;}
  else if ((inval[3] < 604) && (inval[3] > 507)) {kval = 21;}
  else if ((inval[3] < 506) && (inval[3] > 395)) {kval = 1;}
  else if ((inval[3] < 394) && (inval[3] > 261)) {kval = 13;}
  else if ((inval[3] < 260) && (inval[3] > 94)) {kval = 12;}
  else if (inval[3] < 93 ) {kval = 11;}
  else {kval = 100;}

  //Starts key debounce timer
  if ((kval >= 0) && (kval <= 99)) {windex = true;}  

  return kval;
}