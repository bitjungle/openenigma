/**
 * OpenEnigmaGlobals.h
 * Part of Open Enigma M4 Plugboard project
 * See Open_Enigma_M4_Plugboard.ino for more information
 * 2024-12-22 - bitjungle - initial version
 */
#ifndef OPENENIGMA_GLOBALS_H
#define OPENENIGMA_GLOBALS_H
#include "OpenEnigmaConstants.h"

extern int mode;
extern int mdex;
extern bool windex;
extern int keyval;
extern int kvalo;
extern int lampval;
extern int behavior;

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
