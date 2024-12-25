/**
 * OpenEnigmaModes.cpp
 * Part of Open Enigma M4 Plugboard project
 * See Open_Enigma_M4_Plugboard.ino for more information
 * 2024-12-22 - bitjungle
 */
#include "OpenEnigmaConstants.h"
#include "OpenEnigmaModes.h"
#include "OpenEnigmaGlobals.h"
#include "OpenEnigmaLampFunctions.h"
#include "OpenEnigmaDisplayFunctions.h"
#include "OpenEnigmaPlugboardFunctions.h"
#include "OpenEnigmaWheelFunctions.h"

/**
 * Function to change operating Mode
 */
void modeselect(int &mode) {
  mode++;
  if (mode >=6) {mode = 0;}
  if (DEBUG) {Serial.println("modeselect(" + String(mode) + ")");}
  windex = false;
}

/**
 * Mode 0
 * Default Mode: Enigma is a typewriter
 */
void mode0() {
  int lampval = 100; // Default value is for a function key
  if ((keyval >= 0) && (keyval <= 25)) {
    lampval = keyval;
  }
  lampita(lampval);
  delay(3);
  lampitb(lampval);
  marquee();
}

/**
 * Mode 1
 * Select the rotors & the reflector  
 */
void mode1() {
  int index;
  digitalWrite(LED1, HIGH);

  if (windex) {
    if ((keyval == 43) || (keyval == 46)) { // Function button wheel 4 down or up
      if (behavior < 2) { // ENIGMA M4
        wheel[3][0]++;
        windex = false; 
        if (wheel[3][0] > 2) {
          wheel[3][0] = 1; 
          reflect[0]++; 
          if (reflect[0] > 2) {
            reflect[0] = 1;
            }
          } 
      } else { // Enigma M3 or Norenigma
        reflect[0]++; 
        windex = false; 
        if (reflect[0] > 2) {
          reflect[0] = 1;
        } 
      }
    }
  }

  if (windex) {
    if (keyval == 47) {  // Function button wheel 3 up
      for (index = wheel[2][0]; (index == wheel[1][0]) || (index == wheel[0][0]) || (index == wheel[2][0]); index++) {
        if (index > 33 || (behavior == 3 && index > 30)) { //Norenigma (3) only has 5 wheels
          index = 26;
        } 
      }
      wheel[2][0] = index;  
      windex = false;
    }
  }

  if (windex) {
    if (keyval == 48) {  // Function button wheel 2 up
      for(index = wheel[1][0];(index == wheel[2][0]) || (index == wheel[0][0]) || (index == wheel[1][0]); index++) {
        if (index > 33 || (behavior == 3 && index > 30)) { //Norenigma (3) only has 5 wheels
          index = 26;
        } 
      } 
      wheel[1][0] = index;  
      windex = false;
    }
  }
  if (windex) {
    if (keyval == 49) { // Function button wheel 1 up
      for(index = wheel[0][0];(index == wheel[2][0]) || (index == wheel[1][0]) || (index == wheel[0][0]); index++) {
        if (index > 33 || (behavior == 3 && index > 30)) { //Norenigma (3) only has 5 wheels
          index = 26;
        } 
      } 
      wheel[0][0] = index;  
      windex = false;
    }
  }
  if (windex) { // Function button wheel 3 down
    if (keyval == 42) { 
      for(index = wheel[2][0];(index == wheel[1][0]) || (index == wheel[0][0]) || (index == wheel[2][0]); index--) {
        if (index < 28) {
          if (behavior == 3) { //Norenigma (3) only has 5 wheels
            index = 31;
          } else {
            index = 35;
          }
        } 
      } 
      wheel[2][0] = index;  
      windex = false;
    }
  }
  if (windex) { // Function button wheel 2 down
    if (keyval == 41) { 
      for(index = wheel[1][0];(index == wheel[2][0]) || (index == wheel[0][0]) || (index == wheel[1][0]); index--) {
        if (index < 28) {
          if (behavior == 3) { //Norenigma (3) only has 5 wheels
            index = 31;
          } else {
            index = 35;
          }
        } 
      } 
      wheel[1][0] = index;  
      windex = false;
    }
  }
  if (windex) { // Function button wheel 1 down
    if (keyval == 40) { 
      for(index = wheel[0][0];(index == wheel[2][0]) || (index == wheel[1][0]) || (index == wheel[0][0]); index--) {
        if (index < 28) {
          if (behavior == 3) { //Norenigma (3) only has 5 wheels
            index = 31;
          } else {
            index = 35;
          }
        } 
      } 
      wheel[0][0] = index;  
      windex = false;
    }
  }
    
  dig2 = wheel[2][0];  
  dig3 = wheel[1][0]; 
  dig4 = wheel[0][0]; 

  if (behavior < 2) { // Enigma M4
    if (wheel[3][0] == 1 ) { 
      dig1 = 1;
    } else {
      dig1 = 6;
    }
  } else { // Enigma M3 or Norenigma
    dig1 = 36;
  } 

  int lampval = reflect[0]; // Indicate selected reflector B/C in the lamp array
  lampita(lampval);
  delay(3);
  lampitb(lampval);
  nixisend();
  
  digitalWrite(LED1, LOW);
}

/**
 * Mode 2
 * Position the inner setting of each rotor  
 */
void mode2() {
  digitalWrite(LED2, HIGH);
  if (windex) {
    if (behavior < 2) { // Enigma M4
      if (keyval == 46) {
        wheel[3][1]++; 
        if (wheel[3][1] > 25) {
          wheel[3][1] = 0;
        }
      }
    }
    if (keyval == 47) {
      wheel[2][1]++; 
      if (wheel[2][1] > 25) {
        wheel[2][1] = 0;
      }
    }
    if (keyval == 48) {
      wheel[1][1]++; 
      if (wheel[1][1] > 25) {
        wheel[1][1] = 0;
      }
    }
    if (keyval == 49) {
      wheel[0][1]++; 
      if (wheel[0][1] > 25) {
        wheel[0][1] = 0;
      }
    }
    if (behavior < 2) {// Enigma M4
      if (keyval == 43) {
        wheel[3][1]--; 
        if (wheel[3][1] < 0) {
          wheel[3][1] = 25;
        }
      }
    }
    if (keyval == 42) {
      wheel[2][1]--; 
      if (wheel[2][1] < 0) {
        wheel[2][1] = 25;
      }
    }
    if (keyval == 41) {
      wheel[1][1]--; 
      if (wheel[1][1] < 0) {
        wheel[1][1] = 25;
      }
    }
    if (keyval == 40) {
      wheel[0][1]--; 
      if (wheel[0][1] < 0) {
        wheel[0][1] = 25;
      }
    }
    windex = false; 
  }
  
  dig2 = wheel[2][1];  
  dig3 = wheel[1][1]; 
  dig4 = wheel[0][1]; 
  if (behavior < 2) { // Enigma M4
    dig1 = wheel[3][1];
  } else { // Enigma M3 or Norenigma
    dig1 = 36;
  }
  
  nixisend();
  
  dig1 = 37; 
  dig2 = 37; 
  dig3 = 37; 
  dig4 = 37;
  
  digitalWrite(LED2, LOW);
}

/**
 * Mode 3
 * Position the Start character of each Wheel  
 */
void mode3() {
  digitalWrite(LED3, HIGH);
  if (windex) {
    if (behavior < 2) { // Enigma M4
      if (keyval == 46) {
        wheel[3][2]++; 
        if (wheel[3][2] > 25) {
          wheel[3][2] = 0;
        }
      }
    }    // {reflect[1]++; if (reflect[1] > 25) {reflect[1] = 0;}}}
    if (keyval == 47) {
      wheel[2][2]++; 
      if (wheel[2][2] > 25) {
        wheel[2][2] = 0;
      }
    }
    if (keyval == 48) {
      wheel[1][2]++; 
      if (wheel[1][2] > 25) {
        wheel[1][2] = 0;
      }
    }
    if (keyval == 49) {
      wheel[0][2]++; 
      if (wheel[0][2] > 25) {
        wheel[0][2] = 0;
      }
    }
    if (behavior < 2) { // Enigma M4
      if (keyval == 43) {
        wheel[3][2]--; 
        if (wheel[3][2] < 0) {
          wheel[3][2] = 25;
        }
      }
    }    // {reflect[1]--; if (reflect[1] < 0) {reflect[1] = 25;}}}
    if (keyval == 42) {
      wheel[2][2]--; 
      if (wheel[2][2] < 0) {
        wheel[2][2] = 25;
      }
    }
    if (keyval == 41) {
      wheel[1][2]--; 
      if (wheel[1][2] < 0) {
        wheel[1][2] = 25;
      }
    }
    if (keyval == 40) {
      wheel[0][2]--; 
      if (wheel[0][2] < 0) {
        wheel[0][2] = 25;
      }
    }
    windex = false; 
  }
  
  dig4 = wheel[0][2]; 
  dig3 = wheel[1][2]; 
  dig2 = wheel[2][2];  
  dig1 = wheel[3][2];
  if (behavior > 1) {dig1 = 36;} // Enigma M3 or Norenigma
  
  nixisend();
  
  dig1 = 37; 
  dig2 = 37; 
  dig3 = 37; 
  dig4 = 37;
  
  digitalWrite(LED3, LOW);
}

/**
 * Mode 4
 * Define the Plugboard pairs  
 */
void mode4() {
  static int paindex = 0; 
  static int pbindex = 1;

  if (!plugread) {readplugs(); }
  int index = 0;
  digitalWrite(LED4, HIGH);

  if (pluguse <= 9) {
    if (plugval[0][paindex] == 1) {
      for(index = paindex;(index == paindex) || (index == pbindex) || (plugval[0][index] == 1); index++) {
        if (index > 24) {index = -1;}
      } 
      paindex = index;
    }
    if (plugval[0][pbindex] == 1) {
      for(index = pbindex;(index == pbindex) || (index == paindex) || (plugval[0][index] == 1); index++) {
        if (index > 24) {index = -1;}
      } 
      pbindex = index;
    }
  
    if (windex)  { 
      if (keyval == 46) { 
        for (index = paindex;(index == paindex) || (index == pbindex) || (plugval[0][index] == 1); index++) {
          if (index > 24) {index = -1;}
        } 
        paindex = index; 
        windex = false; 
      }
      if (keyval == 43) { 
        for (index = paindex;(index == paindex) || (index == pbindex) || (plugval[0][index] == 1); index--) {
          if (index < 1) {index = 26;}
        } 
        paindex = index; 
        windex = false;  
      }
      if (keyval == 49) { 
        for (index = pbindex;(index == pbindex) || (index == paindex) || (plugval[0][index] == 1); index++) {
          if (index > 24) {index = -1;}
        } 
        pbindex = index; 
        windex = false; 
      }
      if (keyval == 40) { 
        for( index = pbindex;(index == pbindex) || (index == paindex) || (plugval[0][index] == 1); index--) {
          if (index < 1) {index = 26;}
        } 
        pbindex = index; 
        windex = false;  
      }
      if (keyval == 44) { 
        plugval[0][paindex] = 1; 
        plugval[1][paindex] = pbindex; 
        plugval[0][pbindex] = 1; 
        plugval[1][pbindex] = paindex; 
        windex = false; 
        pluguse++;
      }
    }
    
    dig1 = paindex; 
    dig2 = 19; // T 
    dig3 = 14; // O
    dig4 = pbindex; 

    nixisend();
  
    dig1 = 37; 
    dig2 = 37; 
    dig3 = 37; 
    dig4 = 37;
  } else {
    done();
  }
  digitalWrite(LED4, LOW);
}

/**
 * Mode 5
 * This is normal operation mode to Encrypt/Decrypt  
 */
void mode5() {
  int pv = 0;
  int lampval = 100; // Default value is for a function key
  digitalWrite(LED5, HIGH);
  boolean printsignalpath = false;
  
  char signalpath[13]; // Used for serial out for studying the internals of the Enigma

  if ((keyval >= 0) && (keyval <= 25)) {  
    if (windex) { 
      printsignalpath = true;
      procesvala = keyval;  
      indexwheels();
    }

    // Character input
    procesval = procesvala;
    signalpath[0] = CHARS[procesval];

    // Plugboard
    procesval = plugval[1][procesval];
    signalpath[1] = CHARS[procesval];
  
    // Fast rotor
    pv = (procesval + (wheel[0][2] - wheel[0][1]));
    if (pv < 0) {pv = pv + 26;}
    //procesval = ROTORVALS[wheel[0][0] - 27][pv]; 
    procesval = calcprocessval(wheel[0][0], -27, pv);
    if (procesval >= 100) {procesval = procesval - 100;}
    procesval = (procesval - (wheel[0][2] - wheel[0][1]));
    if (procesval < 0) {procesval = procesval + 26;}
    if (procesval > 25) {procesval = procesval - 26;}
    signalpath[2] = CHARS[procesval];
  
    // Middle rotor
    pv = (procesval + (wheel[1][2] - wheel[1][1]));
    if (pv < 0) {pv = pv + 26;}
    //procesval = ROTORVALS[wheel[1][0] - 27][pv]; 
    procesval = calcprocessval(wheel[1][0], -27, pv);
    if (procesval >= 100) {procesval = procesval - 100;}
    procesval = (procesval - (wheel[1][2] - wheel[1][1]));
    if (procesval < 0) {procesval = procesval + 26;}
    if (procesval > 25) {procesval = procesval - 26;}
    signalpath[3] = CHARS[procesval];
  
    // Slow rotor 
    pv = (procesval + (wheel[2][2] - wheel[2][1]));
    if (pv < 0) {pv = pv + 26;}
    //procesval = ROTORVALS[wheel[2][0] - 27][pv]; 
    procesval = calcprocessval(wheel[2][0], -27, pv);
    if (procesval >= 100) {procesval = procesval - 100;}
    procesval = (procesval - (wheel[2][2] - wheel[2][1]));
    if (procesval < 0) {procesval = procesval + 26;}
    if (procesval > 25) {procesval = procesval - 26;}
    signalpath[4] = CHARS[procesval];
  
    // Thin rotor
    if (behavior == 3) { // Norenigma
      signalpath[5] = '-';
      // Reflector UKW
      procesval = NORENIGMA[5][procesval];
      signalpath[6] = CHARS[procesval];
      signalpath[7] = '-';
    } else {// Enigma I, M3 or M4
      pv = (procesval + (wheel[3][2] - wheel[3][1]));
      if (pv < 0) {pv = pv + 26;}
      //procesval = ROTORVALS[wheel[3][0] + 7][pv]; 
      procesval = calcprocessval(wheel[3][0], 7, pv);
      if (procesval >= 100) {procesval = procesval - 100;}
      procesval = (procesval - (wheel[3][2] - wheel[3][1]));
      if (procesval < 0) {procesval = procesval + 26;}
      if (procesval > 25) {procesval = procesval - 26;}
      signalpath[5] = CHARS[procesval];
      // Reflector UKW-B [9] or UKW-C [10] 
      procesval = ROTORVALS[reflect[0] + 9][procesval];
      signalpath[6] = CHARS[procesval];
      // Thin rotor inverted (return)
      pv = (procesval + (wheel[3][2] - wheel[3][1]));
      if (pv < 0) {pv = pv + 26;}
      //procesval = ROTORVALSI[wheel[3][0] + 7][pv];
      procesval = calcprocessvali(wheel[3][0], 7, pv);
      if (procesval >= 100) {procesval = procesval - 100;}
      procesval = (procesval - (wheel[3][2] - wheel[3][1]));
      if (procesval < 0) {procesval = procesval + 26;}
      if (procesval > 25) {procesval = procesval - 26;}
      signalpath[7] = CHARS[procesval];
    }

    // Slow rotor inverted (return)
    pv = (procesval + (wheel[2][2] - wheel[2][1]));
    if (pv < 0) {pv = pv + 26;}
    //procesval = ROTORVALSI[wheel[2][0] - 27][pv]; 
    procesval = calcprocessvali(wheel[2][0], -27, pv);
    if (procesval >= 100) {procesval = procesval - 100;}
    procesval = (procesval - (wheel[2][2] - wheel[2][1]));
    if (procesval < 0) {procesval = procesval + 26;}
    if (procesval > 25) {procesval = procesval - 26;}
    signalpath[8] = CHARS[procesval];

    // Middle rotor inverted (return)
    pv = (procesval + (wheel[1][2] - wheel[1][1]));
    if (pv < 0) {pv = pv + 26;}
    //procesval = ROTORVALSI[wheel[1][0] - 27][pv]; 
    procesval = calcprocessvali(wheel[1][0], -27, pv);
    if (procesval >= 100) {procesval = procesval - 100;}
    procesval = (procesval - (wheel[1][2] - wheel[1][1]));
    if (procesval < 0) {procesval = procesval + 26;}
    if (procesval > 25) {procesval = procesval - 26;}
    signalpath[9] = CHARS[procesval];
  
    // Fast rotor inverted (return)
    pv = (procesval + (wheel[0][2] - wheel[0][1]));
    if (pv < 0) {pv = pv + 26;}
    //procesval = ROTORVALSI[wheel[0][0] - 27][pv]; 
    procesval = calcprocessvali(wheel[0][0], -27, pv);
    if (procesval >= 100) {procesval = procesval - 100;}
    procesval = (procesval - (wheel[0][2] - wheel[0][1]));
    if (procesval < 0) {procesval = procesval + 26;}
    if (procesval > 25) {procesval = procesval - 26;}
    signalpath[10] = CHARS[procesval];

    // Plugboard
    procesval = plugval[1][procesval];

    // Encrypted/decrypted output
    lampval = procesval;

    signalpath[11] = CHARS[procesval]; 
    signalpath[12] = '\0';   
    if (DEBUG && printsignalpath) {
      Serial.print("mode5() : "); 
      Serial.println(signalpath);
    }    
  }

  windex = false;

  // Updating displayed wheel positions
  dig4 = wheel[0][2]; 
  dig3 = wheel[1][2]; 
  dig2 = wheel[2][2];  
  dig1 = wheel[3][2];
  if (behavior > 1) {dig1 = 36;} // Enigma M3 or Norenigma
  
  lampita(lampval);
  delay(3);
  lampitb(lampval);
  
  nixisend();
  
  dig4 = 37;
  dig3 = 37; 
  dig2 = 37; 
  dig1 = 37; 
  
  digitalWrite(LED5, LOW);
}

/**
 * 
 */
int calcprocessval(int ringpos, int offset, int pv) {
  if (behavior == 3) { // Emulating a Norenigma
    return NORENIGMA[ringpos + offset][pv];
  } else { // Enigma I, M3 or M4
    return ROTORVALS[ringpos + offset][pv];
  }
}

/**
 * 
 */
int calcprocessvali(int ringpos, int offset, int pv) {
  if (behavior == 3) { // Emulating a Norenigma
    return NORENIGMAI[ringpos + offset][pv];
  } else { // Enigma I, M3 or M4
    return ROTORVALSI[ringpos + offset][pv];
  }
}
