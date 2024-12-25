/**
 * OpenEnigmaWheelFunctions.cpp
 * Part of Open Enigma M4 Plugboard project
 * See Open_Enigma_M4_Plugboard.ino for more information
 * 2024-12-22 - bitjungle
 */
#include "OpenEnigmaConstants.h"
#include "OpenEnigmaGlobals.h"
#include "OpenEnigmaWheelFunctions.h"
#include "OpenEnigmaWheelWiring.h"

/**
 * @brief Rotates the wheels of the Enigma machine and handles the turnover mechanism.
 * 
 * This function simulates the rotation of the wheels in an Enigma machine. It checks 
 * for the turnover positions of the wheels and rotates them accordingly. The function 
 * also handles the double-stepping anomaly of the Enigma machine.
 * 
 * The function uses the global variables:
 * - `wheel`: A 2D array representing the wheels of the Enigma machine. Each wheel has 
 *   three values: the wheel number, the inner ring position, and the outer ring position.
 * - `ROTORVALS`: A 2D array containing the turnover positions for each wheel.
 * - `behavior`: An integer indicating the behavior mode of the machine.
 * - `DEBUG`: A boolean indicating whether to print debug information.
 * 
 * The function performs the following steps:
 * 1. Checks if the first wheel (wheel[1]) is at a turnover position and sets the 
 *    `windex1` flag if true. If in double-stepping mode, also sets the `windex2` flag.
 * 2. Checks if the second wheel (wheel[0]) is at a turnover position and sets the 
 *    `windex1` flag if true.
 * 3. Rotates the first wheel (wheel[0]).
 * 4. If the `windex1` flag is set, checks if the first wheel (wheel[1]) is at a turnover 
 *    position and sets the `windex2` flag if true, then rotates the second wheel (wheel[1]).
 * 5. If the `windex2` flag is set, rotates the third wheel (wheel[2]).
 * 6. Resets the `windex1` and `windex2` flags.
 * 7. If `DEBUG` is true, prints the current positions of the wheels.
 */
void indexwheels(bool &windex) {
  static boolean windex1 = false;
  static boolean windex2 = false;

  if (behavior > 0) { 
    if (ROTORVALS[wheel[1][0]-27][wheel[1][2]] >= 100) {
      windex1 = true;
      if (behavior < 2) { // Double stepping?
        windex2 = true;
      }
    }
  }
  if (ROTORVALS[wheel[0][0]-27][wheel[0][2]] >= 100) {
    windex1 = true; // We have a wheel 2 turnover
  } 
  wheel[0][2]++; // Rotate wheel 1
  if (wheel[0][2] > 25) {
    wheel[0][2] = 0; // Wheel 1 has completed a full revolution
  }
  windex = false;
  if (windex1) {
    if (ROTORVALS[wheel[1][0]-27][wheel[1][2]] >= 100) {
      windex2 = true; // We have a wheel 3 turnover
    }
    wheel[1][2]++; // Rotate wheel 2
    if (wheel[1][2] > 25) {
      wheel[1][2] = 0; // Wheel 2 has completed a full revolution
    }
  }
  windex1 = false; // Resetting turnover flag
  if (windex2){
    wheel[2][2]++; // Rotate wheel 3
    if (wheel[2][2] > 25) {
      wheel[2][2] = 0; // Wheel 3 has completed a full revolution
    }
    windex2 = false; // Resetting turnover flag
  } 
  if (DEBUG) {
    Serial.print("indexwheels() wheels      : "); 
    Serial.print(wheel[3][0]);  Serial.print("   "); 
    Serial.print(wheel[2][0]);  Serial.print("   "); 
    Serial.print(wheel[1][0]);  Serial.print("   "); 
    Serial.println(wheel[0][0]);
    Serial.print("indexwheels() inring pos  : "); 
    Serial.print(wheel[3][1]);  Serial.print("    "); 
    Serial.print(wheel[2][1]);  Serial.print("    "); 
    Serial.print(wheel[1][1]);  Serial.print("    "); 
    Serial.println(wheel[0][1]);
    Serial.print("indexwheels() outring pos : "); 
    Serial.print(wheel[3][2]);  Serial.print("    "); 
    Serial.print(wheel[2][2]);  Serial.print("    "); 
    Serial.print(wheel[1][2]);  Serial.print("    "); 
    Serial.println(wheel[0][2]);
  }
}