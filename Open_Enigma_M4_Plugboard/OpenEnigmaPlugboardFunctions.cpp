/**
 * OpenEnigmaPlugboardFunctions.cpp
 * Part of Open Enigma M4 Plugboard project
 * See Open_Enigma_M4_Plugboard.ino for more information
 * 2024-12-22 - bitjungle
 */
#include "OpenEnigmaConstants.h"
#include "OpenEnigmaGlobals.h"
#include "OpenEnigmaPlugboardFunctions.h"

/**
 * Read the plugboard settings
 */   
void readplugs(bool &plugread) {
  for (int index = 0 ; index <= 24; index++) {
    pinMode(PLUGPINS[index], OUTPUT);
    digitalWrite(PLUGPINS[index], LOW); 
    
    for (int indexb = (index + 1); indexb <= 25; indexb++) {
      int plugvar = digitalRead(PLUGPINS[indexb]);
      if (plugvar == 0) {
        pluguse++;
        plugval[0][index] = 1; 
        plugval[0][indexb] = 1;
        plugval[1][index] = indexb; 
        plugval[1][indexb] = index;
        if (DEBUG) {
          Serial.print("readplugs() : "); 
          Serial.print(CHARS[index]); 
          Serial.print(" -> "); 
          Serial.println(CHARS[indexb]);
        }
      }
    }
    pinMode(PLUGPINS[index], INPUT);
  }
  plugread = true;
}