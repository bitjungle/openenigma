/**
 * OpenEnigmaLampFunctions.cpp
 * Part of Open Enigma M4 Plugboard project
 * See Open_Enigma_M4_Plugboard.ino for more information
 * 2024-12-22 - bitjungle
 */
#include "OpenEnigmaConstants.h"
#include "OpenEnigmaGlobals.h"

/**
 * Helper Function to light the proper key 
 */
void lampita() {
  if (lampval <= 25) {
  digitalWrite(LAMPARRAY[lampval][0],0);
  digitalWrite(LAMPARRAY[lampval][1],0);
  if (DEBUG && windex) {Serial.print("lampita() : lampval=");Serial.println(lampval);}
  delay(1);
  }
}

/**
 * 
 */
void lampitb(){
  if (lampval <= 25) {
    digitalWrite(LAMPARRAY[lampval][0],1);
    digitalWrite(LAMPARRAY[lampval][1],1);  
    if (DEBUG && windex) {Serial.print("lampitb() : lampval=");Serial.println(lampval);}
  }
}
