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
void modeselect(int &mode, bool &windex);
void mode0(int keyval, bool &windex);
void mode1(int keyval, bool &windex);
void mode2(int keyval, bool &windex);
void mode3(int keyval, bool &windex);
void mode4(int keyval, bool &windex);
void mode5(int keyval, bool &windex);
int calcprocessval(int ringpos, int offset, int pv);
int calcprocessvali(int ringpos, int offset, int pv);

#endif // OPENENIGMA_MODES_H