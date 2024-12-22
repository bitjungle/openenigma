/**
 * OpenEnigmaGlobals.h
 * Part of Open Enigma M4 Plugboard project
 * See Open_Enigma_M4_Plugboard.ino for more information
 * 2024-12-22 - bitjungle
 */
#ifndef OPENENIGMA_GLOBALS_H
#define OPENENIGMA_GLOBALS_H
#include "OpenEnigmaConstants.h"

extern unsigned long time;// Number of milliseconds since start
extern unsigned long otime;
extern int behavior; // Switch between various Enigma models 


extern int mode;
extern int mdex;
extern bool windex;
extern int keyval;
extern int kvalo;
extern int lampval;

extern int wheel[4][3];
extern int reflect[2];

extern int procesval;
extern int procesvala;

extern boolean plugread;
extern int pluguse;

extern int plugval[2][NUMCHARS];

// Define each sixteen-segment display character
extern int dig1;
extern int dig2;
extern int dig3;
extern int dig4;

#endif // OPENENIGMA_GLOBALS_H
