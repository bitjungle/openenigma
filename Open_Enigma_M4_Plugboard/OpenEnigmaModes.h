/**
 * OpenEnigmaModes.h
 * Part of Open Enigma M4 Plugboard project
 * See Open_Enigma_M4_Plugboard.ino for more information
 * 2024-12-22 - bitjungle
 */
#ifndef OPENENIGMA_MODES_H
#define OPENENIGMA_MODES_H
#include "OpenEnigmaWheelWiring.h"

// Declare functions
void modeselect(int &mode);
void mode0();
void mode1();
void mode2();
void mode3();
void mode4();
void mode5();
int calcprocessval(int ringpos, int offset, int pv);
int calcprocessvali(int ringpos, int offset, int pv);

#endif // OPENENIGMA_MODES_H