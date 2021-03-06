#include <SoftwareSerial.h>

/** S&T GeoTronics LLC  **** OPEN ENIGMA ****
 *
 *   This code is provided as is. S&T GeoTronics LLC, it's Partners and Associates provide it freely,
 *   with no warranty of it's functionality, fitness for use or usefulness. In no way shall S&T GeoTronics LLC,
 *   it's Partners and Associates be liable for the use, misuse, or damages of any kind due to the use or
 *   misuse of this code or any associated hardware provided.
 *   
 *   Enigma Code. This Arduino Mega custom shield is programmed to replicate
 *   exactly the behavior of a true German Enigma M4 machine.
 *   It uses 4 sixteen-segment units, 5 LEDs, 26 lamps setup as keyboard, 26 keyboard buttons
 *   & 10 function keys. The 115 light emitting diodes are multi-plexed to minimize the 
 *   amount of pins needed down to 38 and all 36 pushbuttons keys are sharing a total of 4 pins.
 * 
 *   - Designed, assembled & programmed by Marc Tessier & James Sanderson 9/20/13
 *   - Modified for our Prototype PCB pair on 12/4/13.
 *   - Modified to obey doublesteping and have M3 function 22 MAR 14
 *   - Code formatting, cleanup and documentation, serial output - bitjungle 2019-01-06
 *   - Norenigma emulation - bitjungle 2019-01-26
 *
 *   This code shall remain in public domain as regulated under the creative commons licence.
 */

// Define global variables

const bool DEBUG = false; // Set to false if serial output is not needed

const int NUMCHARS = 26;    // Number of characters available on the Enigma
const char CHARS[NUMCHARS+1] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; // Including NULL 

unsigned long time = millis();// Number of milliseconds since start
unsigned long otime = time;   // Used in keyboard debounce code

const int INPINS[4] = {A0, A1, A2, A3};

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

// Define Plug Board Pins
const int PLUGPINS[NUMCHARS] = {A14,49,A12,A15,7,53,50,52,3,A6,A4,A11,A9,A10,8,A7,6,4,51,10,9,A5,5,A8,A13,2} ;

// Define each sixteen-segment display character
int dig1 = 37;
int dig2 = 37;
int dig3 = 37;
int dig4 = 37;

const int EMULATORS = 4; // Number of implementet emulators
const int MARQUEECHARS = 21;
const int MARQUEETEXT[EMULATORS][MARQUEECHARS] = {{36,36,36,36,4,13,8,6,12,0,36,12,30,36,36,36,36,36,36,36,36},  //Enigma M4
                                                  {36,36,36,36,4,13,8,6,12,0,36,12,30,36,3,1,11,36,36,36,36},    //Enigma M4 DBL
                                                  {36,36,36,36,4,13,8,6,12,0,36,12,29,36,36,36,36,36,36,36,36},  //Enigma M3
                                                  {36,36,36,36,13,14,17,4,13,8,6,12,0,36,36,36,36,36,36,36,36}}; //Norenigma

// Define the sixteen-segment display LED Pins as 2 Arrays
const int SEGMENTPINS[17] = {24,22,25,31,38,36,32,30,28,26,23,27,33,35,34,29,37}; //cathode array
const int ANODEPINS[4] = {39,41,43,45}; //anode array common anode

// Define the three common anode pins for the lamp array
const int LAMPPINS[3] = {11,12,13};

// Define the 26 Lamps as a 2D Array {anode,cathode}
const int LAMPARRAY[NUMCHARS][2] = {{12,38}, // A
                                    {13,29}, // B
                                    {13,33}, // C
                                    {12,35}, // D
                                    {11,35}, // E
                                    {12,33}, // F
                                    {12,31}, // G
                                    {12,29}, // H
                                    {11,25}, // I
                                    {12,27}, // J
                                    {12,25}, // K
                                    {13,23}, // L
                                    {13,25}, // M
                                    {13,27}, // N
                                    {11,23}, // O
                                    {13,38}, // P
                                    {11,38}, // Q
                                    {11,33}, // R
                                    {12,37}, // S
                                    {11,31}, // T
                                    {11,27}, // U
                                    {13,31}, // V
                                    {11,37}, // W
                                    {13,35}, // X
                                    {13,37}, // Y
                                    {11,29}};// Z

 // Define each LTP587P Segments: A,B,C,D,E,F,G,H,K,M,N,P,R,S,T,U,dp               
const boolean SEGMENTVALS[40][17] = { { 0,0,0,0,1,1,0,0,1,1,1,0,1,1,1,0,1 },  // = A 0
                                      { 0,0,0,0,0,0,1,1,1,0,1,0,1,0,1,1,1 },  // = B 1
                                      { 0,0,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1 },  // = C 2
                                      { 0,0,0,0,0,0,1,1,1,0,1,1,1,0,1,1,1 },  // = D 3
                                      { 0,0,1,1,0,0,0,0,1,1,1,0,1,1,1,0,1 },  // = E 4
                                      { 0,0,1,1,1,1,0,0,1,1,1,0,1,1,1,0,1 },  // = F 5
                                      { 0,0,1,0,0,0,0,0,1,1,1,0,1,1,1,1,1 },  // = G 6
                                      { 1,1,0,0,1,1,0,0,1,1,1,0,1,1,1,0,1 },  // = H 7
                                      { 0,0,1,1,0,0,1,1,1,0,1,1,1,0,1,1,1 },  // = I 8
                                      { 1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1 },  // = J 9
                                      { 1,1,1,1,1,1,0,0,1,1,0,1,0,1,1,0,1 },  // = K 10
                                      { 1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1 },  // = L 11 
                                      { 1,1,0,0,1,1,0,0,0,1,0,1,1,1,1,1,1 },  // = M 12
                                      { 1,1,0,0,1,1,0,0,0,1,1,1,0,1,1,1,1 },  // = N 13
                                      { 0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1 },  // = O 14
                                      { 0,0,0,1,1,1,0,0,1,1,1,0,1,1,1,0,1 },  // = P 15
                                      { 0,0,0,0,0,0,0,0,1,1,1,1,0,1,1,1,1 },  // = Q 16
                                      { 0,0,0,1,1,1,0,0,1,1,1,0,0,1,1,0,1 },  // = R 17
                                      { 0,0,1,0,0,0,1,0,1,1,1,0,1,1,1,0,1 },  // = S 18
                                      { 0,0,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1 },  // = T 19
                                      { 1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1 },  // = U 20
                                      { 1,1,1,1,1,1,0,0,1,1,0,1,1,1,0,1,1 },  // = V 21
                                      { 1,1,0,0,1,1,0,0,1,1,1,1,0,1,0,1,1 },  // = W 22
                                      { 1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1 },  // = X 23
                                      { 1,1,1,1,1,1,1,1,0,1,0,1,1,0,1,1,1 },  // = Y 24
                                      { 0,0,1,1,0,0,1,1,1,1,0,1,1,1,0,1,1 },  // = Z 25
                                      { 0,0,0,0,0,0,0,0,1,1,0,1,1,1,0,1,1 },  // = 0 26
                                      { 1,1,0,0,1,1,1,1,1,1,0,1,1,1,1,1,1 },  // = 1 27
                                      { 0,0,0,1,0,0,0,1,1,1,1,0,1,1,1,0,1 },  // = 2 28
                                      { 0,0,0,0,0,0,1,1,1,1,1,0,1,1,1,1,1 },  // = 3 29
                                      { 1,1,0,0,1,1,1,0,1,1,1,0,1,1,1,0,1 },  // = 4 30
                                      { 0,0,1,0,0,0,1,0,1,1,1,0,1,1,1,0,1 },  // = 5 31
                                      { 0,0,1,0,0,0,0,0,1,1,1,0,1,1,1,0,1 },  // = 6 32
                                      { 0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1 },  // = 7 33
                                      { 0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,1 },  // = 8 34
                                      { 0,0,0,0,0,0,1,0,1,1,1,0,1,1,1,0,1 },  // = 9 35
                                      { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },  // = Space 36
                                      { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },  // = Full Lit 37
                                      { 1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1 },  // = SS 38
                                      { 0,1,1,1,0,0,0,1,0,0,1,1,0,1,1,0,1} }; // = & 39

// Define the 5 Mode LEDs 
const int LED1 = 40; // Mode 1: Rotors
const int LED2 = 42; // Mode 2: Inrings
const int LED3 = 44; // Mode 3: Outrings
const int LED4 = 46; // Mode 4: Plugs
const int LED5 = 48; // Mode 5: Run

// ENIGMA M4 WHEEL WIRING - https://cryptomuseum.com/crypto/enigma/wiring.htm#6 
// The wiring of the first five wheels are identical on Enigma I, M3 and M4.
// The Enigma M4 came standard with thin reflector b and extra wheel β (Beta) 
// fitted. They were wired in such a way that together, with the β-wheel set 
// to 'A', the combination behaved exactly like UKW-B in the Enigma M3 and 
// Enigma I. This made the machine backwards compatible. 
// =============================================================================
// Wheel   ABCDEFGHIJKLMNOPQRSTUVWXYZ     Notch     Turnover     #
// ---------------------------------------------------------------
// ETW     ABCDEFGHIJKLMNOPQRSTUVWXYZ            
// I       EKMFLGDQVZNTOWYHXUSPAIBRCJ     Y         Q           1
// II      AJDKSIRUXBLHWTMCQGZNPYFVOE     M         E           1
// III     BDFHJLCPRTXVZNYEIWGAKMUSQO     D         V           1
// IV      ESOVPZJAYQUIRHXLNFTGKDCMWB     R         J           1
// V       VZBRGITYUPSDNHLXAWMJQOFECK     H         Z           1
// VI      JPGVOUMFYQBENHZRDKASXLICTW     HU        ZM          2
// VII     NZJHGRCXMYSWBOUFAIVLPEKQDT     HU        ZM          2
// VIII    FKQHTLXOCBJSPDZRAMEWNIUYGV     HU        ZM          2
// Beta    LEYJVCNIXWPBQMDRTAKZGFUHOS            
// Gamma   FSOKANUERHMBTIYCWLQPZXVGJD            
// UKW-B   ENKQAUYWJICOPBLMDXZVFTHRGS            
// UKW-C   RDOBJNTKVEHMLFCWZAXGYIPSUQ            
const int ROTORVALS[12][78] = { { 4,10,12,5,11,6,3,16,21,25,13,19,14,22,24,7,123,20,18,15,0,8,1,17,2,9,
                                  4,10,12,5,11,6,3,16,21,25,13,19,14,22,24,7,123,20,18,15,0,8,1,17,2,9,
                                  4,10,12,5,11,6,3,16,21,25,13,19,14,22,24,7,123,20,18,15,0,8,1,17,2,9 }, // I (indexwheel 27)

                                { 0,9,3,10,118,8,17,20,23,1,11,7,22,19,12,2,16,6,25,13,15,24,5,21,14,4,
                                  0,9,3,10,118,8,17,20,23,1,11,7,22,19,12,2,16,6,25,13,15,24,5,21,14,4,
                                  0,9,3,10,118,8,17,20,23,1,11,7,22,19,12,2,16,6,25,13,15,24,5,21,14,4 }, // II  (indexwheel 28)
                                  
                                { 1,3,5,7,9,11,2,15,17,19,23,21,25,13,24,4,8,22,6,0,10,112,20,18,16,14,
                                  1,3,5,7,9,11,2,15,17,19,23,21,25,13,24,4,8,22,6,0,10,112,20,18,16,14,
                                  1,3,5,7,9,11,2,15,17,19,23,21,25,13,24,4,8,22,6,0,10,112,20,18,16,14 }, // III (indexwheel 29)
                                  
                                { 4,18,14,21,15,25,9,0,24,116,20,8,17,7,23,11,13,5,19,6,10,3,2,12,22,1,
                                  4,18,14,21,15,25,9,0,24,116,20,8,17,7,23,11,13,5,19,6,10,3,2,12,22,1,
                                  4,18,14,21,15,25,9,0,24,116,20,8,17,7,23,11,13,5,19,6,10,3,2,12,22,1 }, // IV (indexwheel 30)
                                    
                                { 21,25,1,17,6,8,19,24,20,15,18,3,13,7,11,23,0,22,12,9,16,14,5,4,2,110,
                                  21,25,1,17,6,8,19,24,20,15,18,3,13,7,11,23,0,22,12,9,16,14,5,4,2,110,
                                  21,25,1,17,6,8,19,24,20,15,18,3,13,7,11,23,0,22,12,9,16,14,5,4,2,110  }, // V (indexwheel 31)
                                  
                                { 9,15,6,21,14,20,12,5,24,16,1,4,113,7,25,17,3,10,0,18,23,11,8,2,19,122,
                                  9,15,6,21,14,20,12,5,24,16,1,4,113,7,25,17,3,10,0,18,23,11,8,2,19,122,
                                  9,15,6,21,14,20,12,5,24,16,1,4,113,7,25,17,3,10,0,18,23,11,8,2,19,122 }, // VI (indexwheel 32)
                                  
                                { 13,25,9,7,6,17,2,23,12,24,18,22,101,14,20,5,0,8,21,11,15,4,10,16,3,119,
                                  13,25,9,7,6,17,2,23,12,24,18,22,101,14,20,5,0,8,21,11,15,4,10,16,3,119,
                                  13,25,9,7,6,17,2,23,12,24,18,22,101,14,20,5,0,8,21,11,15,4,10,16,3,119 }, // VII (indexwheel 33)
                                  
                                { 5,10,16,7,19,11,23,14,2,1,9,18,115,3,25,17,0,12,4,22,13,8,20,24,6,121,
                                  5,10,16,7,19,11,23,14,2,1,9,18,115,3,25,17,0,12,4,22,13,8,20,24,6,121,
                                  5,10,16,7,19,11,23,14,2,1,9,18,115,3,25,17,0,12,4,22,13,8,20,24,6,121 }, // VIII (indexwheel 34)
                                  
                                { 11,4,24,9,21,2,13,8,23,22,15,1,16,12,3,17,19,0,10,25,6,5,20,7,14,18,
                                  11,4,24,9,21,2,13,8,23,22,15,1,16,12,3,17,19,0,10,25,6,5,20,7,14,18,
                                  11,4,24,9,21,2,13,8,23,22,15,1,16,12,3,17,19,0,10,25,6,5,20,7,14,18 },   // Beta (indexwheel 1)
                                  
                                { 5,18,14,10,0,13,20,4,17,7,12,1,19,8,24,2,22,11,16,15,25,23,21,6,9,3,
                                  5,18,14,10,0,13,20,4,17,7,12,1,19,8,24,2,22,11,16,15,25,23,21,6,9,3,
                                  5,18,14,10,0,13,20,4,17,7,12,1,19,8,24,2,22,11,16,15,25,23,21,6,9,3 },   // Gamma (indexwheel 2)
                                  
                                { 4,13,10,16,0,20,24,22,9,8,2,14,15,1,11,12,3,23,25,21,5,19,7,17,6,18,
                                  4,13,10,16,0,20,24,22,9,8,2,14,15,1,11,12,3,23,25,21,5,19,7,17,6,18,
                                  4,13,10,16,0,20,24,22,9,8,2,14,15,1,11,12,3,23,25,21,5,19,7,17,6,18},   // UKW-B
                                  
                                { 17,3,14,1,9,13,19,10,21,4,7,12,11,5,2,22,25,0,23,6,24,8,15,18,20,16,
                                  17,3,14,1,9,13,19,10,21,4,7,12,11,5,2,22,25,0,23,6,24,8,15,18,20,16,
                                  17,3,14,1,9,13,19,10,21,4,7,12,11,5,2,22,25,0,23,6,24,8,15,18,20,16 }    // UKW-C 
                              };
                                        
// Inverted rotor values
const int ROTORVALSI[10][78] = { { 20,22,24,6,0,3,5,15,21,25,1,4,2,10,12,19,7,23,18,11,17,8,13,16,14,9,
                                   20,22,24,6,0,3,5,15,21,25,1,4,2,10,12,19,7,23,18,11,17,8,13,16,14,9,
                                   20,22,24,6,0,3,5,15,21,25,1,4,2,10,12,19,7,23,18,11,17,8,13,16,14,9  }, // I i
                            
                                 { 0,9,15,2,25,22,17,11,5,1,3,10,14,19,24,20,16,6,4,13,7,23,12,8,21,18,
                                   0,9,15,2,25,22,17,11,5,1,3,10,14,19,24,20,16,6,4,13,7,23,12,8,21,18,
                                   0,9,15,2,25,22,17,11,5,1,3,10,14,19,24,20,16,6,4,13,7,23,12,8,21,18  }, // II i
                                
                                 { 19,0,6,1,15,2,18,3,16,4,20,5,21,13,25,7,24,8,23,9,22,11,17,10,14,12,
                                   19,0,6,1,15,2,18,3,16,4,20,5,21,13,25,7,24,8,23,9,22,11,17,10,14,12,
                                   19,0,6,1,15,2,18,3,16,4,20,5,21,13,25,7,24,8,23,9,22,11,17,10,14,12 },   // III i
                                
                                 { 7,25,22,21,0,17,19,13,11,6,20,15,23,16,2,4,9,12,1,18,10,3,24,14,8,5,
                                   7,25,22,21,0,17,19,13,11,6,20,15,23,16,2,4,9,12,1,18,10,3,24,14,8,5,
                                   7,25,22,21,0,17,19,13,11,6,20,15,23,16,2,4,9,12,1,18,10,3,24,14,8,5 },  // IV i
                                  
                                 { 16,2,24,11,23,22,4,13,5,19,25,14,18,12,21,9,20,3,10,6,8,0,17,15,7,1,
                                   16,2,24,11,23,22,4,13,5,19,25,14,18,12,21,9,20,3,10,6,8,0,17,15,7,1,
                                   16,2,24,11,23,22,4,13,5,19,25,14,18,12,21,9,20,3,10,6,8,0,17,15,7,1 },  // V i
                                  
                                 { 18,10,23,16,11,7,2,13,22,0,17,21,06,12,4,1,9,15,19,24,5,3,25,20,8,14,   // Error here? 
                                   18,10,23,16,11,7,2,13,22,0,17,21,06,12,4,1,9,15,19,24,5,3,25,20,8,14,   // Value 06?
                                   18,10,23,16,11,7,2,13,22,0,17,21,06,12,4,1,9,15,19,24,5,3,25,20,8,14 }, // VI i
                              
                                 { 16,12,6,24,21,15,4,3,17,2,22,19,8,0,13,20,23,5,10,25,14,18,11,7,9,1,
                                   16,12,6,24,21,15,4,3,17,2,22,19,8,0,13,20,23,5,10,25,14,18,11,7,9,1,
                                   16,12,6,24,21,15,4,3,17,2,22,19,8,0,13,20,23,5,10,25,14,18,11,7,9,1 },  // VII i
                                
                                 { 16,9,8,13,18,0,24,3,21,10,1,5,17,20,7,12,2,15,11,4,22,25,19,6,23,14,
                                   16,9,8,13,18,0,24,3,21,10,1,5,17,20,7,12,2,15,11,4,22,25,19,6,23,14,
                                   16,9,8,13,18,0,24,3,21,10,1,5,17,20,7,12,2,15,11,4,22,25,19,6,23,14 },  // VIII i
                                  
                                 { 17,11,5,14,1,21,20,23,7,3,18,0,13,6,24,10,12,15,25,16,22,4,9,8,2,19,
                                   17,11,5,14,1,21,20,23,7,3,18,0,13,6,24,10,12,15,25,16,22,4,9,8,2,19,
                                   17,11,5,14,1,21,20,23,7,3,18,0,13,6,24,10,12,15,25,16,22,4,9,8,2,19 },  // Beta i
                                  
                                 { 4,11,15,25,7,0,23,9,13,24,3,17,10,5,2,19,18,8,1,12,6,22,16,21,14,20,
                                   4,11,15,25,7,0,23,9,13,24,3,17,10,5,2,19,18,8,1,12,6,22,16,21,14,20,
                                   4,11,15,25,7,0,23,9,13,24,3,17,10,5,2,19,18,8,1,12,6,22,16,21,14,20 }   // Gamma i
                                }; 

// // NORWAY ENIGMA WHEEL WIRING - https://cryptomuseum.com/crypto/enigma/wiring.htm#3                                     
const int NORENIGMA[6][78] = { { 22,19,14,10,0,18,20,24,21,17,1,23,9,7,16,2,115,25,4,5,12,3,8,13,11,6,
                                 22,19,14,10,0,18,20,24,21,17,1,23,9,7,16,2,115,25,4,5,12,3,8,13,11,6,
                                 22,19,14,10,0,18,20,24,21,17,1,23,9,7,16,2,115,25,4,5,12,3,8,13,11,6 }, // I (indexwheel 27)

                               { 6,9,11,15,120,1,18,22,4,12,2,19,16,21,7,23,0,14,5,25,3,17,10,24,13,8,
                                 6,9,11,15,120,1,18,22,4,12,2,19,16,21,7,23,0,14,5,25,3,17,10,24,13,8,
                                 6,9,11,15,120,1,18,22,4,12,2,19,16,21,7,23,0,14,5,25,3,17,10,24,13,8 }, // II  (indexwheel 28)
                                
                               { 9,22,5,12,7,13,1,15,20,18,3,24,19,8,23,21,25,6,17,16,11,100,14,4,10,2,
                                 9,22,5,12,7,13,1,15,20,18,3,24,19,8,23,21,25,6,17,16,11,100,14,4,10,2,
                                 9,22,5,12,7,13,1,15,20,18,3,24,19,8,23,21,25,6,17,16,11,100,14,4,10,2 }, // III (indexwheel 29)
                                
                               { 4,18,14,21,15,25,9,0,24,116,20,8,17,7,23,11,13,5,19,6,10,3,2,12,22,1,
                                 4,18,14,21,15,25,9,0,24,116,20,8,17,7,23,11,13,5,19,6,10,3,2,12,22,1,
                                 4,18,14,21,15,25,9,0,24,116,20,8,17,7,23,11,13,5,19,6,10,3,2,12,22,1 }, // IV (indexwheel 30)
                                  
                               { 7,4,9,23,16,14,19,25,1,21,5,3,0,18,2,8,11,22,15,6,24,13,12,20,17,110,
                                 7,4,9,23,16,14,19,25,1,21,5,3,0,18,2,8,11,22,15,6,24,13,12,20,17,110,
                                 7,4,9,23,16,14,19,25,1,21,5,3,0,18,2,8,11,22,15,6,24,13,12,20,17,110  }, // V (indexwheel 31)
                                
                               { 12,14,22,9,24,15,20,23,13,3,18,17,0,8,1,5,21,11,10,25,6,16,2,7,4,19,
                                 12,14,22,9,24,15,20,23,13,3,18,17,0,8,1,5,21,11,10,25,6,16,2,7,4,19,
                                 12,14,22,9,24,15,20,23,13,3,18,17,0,8,1,5,21,11,10,25,6,16,2,7,4,19 }    // UKW
                              };

// Inverted rotor values
const int NORENIGMAI[5][78] = { { 4,10,15,21,18,19,25,13,22,12,3,24,20,23,2,16,14,9,5,1,6,8,0,11,7,17,
                                  4,10,15,21,18,19,25,13,22,12,3,24,20,23,2,16,14,9,5,1,6,8,0,11,7,17,
                                  4,10,15,21,18,19,25,13,22,12,3,24,20,23,2,16,14,9,5,1,6,8,0,11,7,17  }, // I i
                          
                                { 16,5,10,20,8,18,0,14,25,1,22,2,9,24,17,3,12,21,6,11,4,13,7,15,23,19,
                                  16,5,10,20,8,18,0,14,25,1,22,2,9,24,17,3,12,21,6,11,4,13,7,15,23,19,
                                  16,5,10,20,8,18,0,14,25,1,22,2,9,24,17,3,12,21,6,11,4,13,7,15,23,19  }, // II i
                              
                                { 21,6,25,10,23,2,17,4,13,0,24,20,3,5,22,7,19,18,9,12,8,15,1,14,11,16,
                                  21,6,25,10,23,2,17,4,13,0,24,20,3,5,22,7,19,18,9,12,8,15,1,14,11,16,
                                  21,6,25,10,23,2,17,4,13,0,24,20,3,5,22,7,19,18,9,12,8,15,1,14,11,16 },   // III i
                              
                              
                                { 7,25,22,21,0,17,19,13,11,6,20,15,23,16,2,4,9,12,1,18,10,3,24,14,8,5,
                                  7,25,22,21,0,17,19,13,11,6,20,15,23,16,2,4,9,12,1,18,10,3,24,14,8,5,
                                  7,25,22,21,0,17,19,13,11,6,20,15,23,16,2,4,9,12,1,18,10,3,24,14,8,5 },  // IV i
                                
                                { 12,8,14,11,1,10,19,0,15,2,25,16,22,21,5,18,4,24,13,6,23,9,17,3,20,7,
                                  12,8,14,11,1,10,19,0,15,2,25,16,22,21,5,18,4,24,13,6,23,9,17,3,20,7,
                                  12,8,14,11,1,10,19,0,15,2,25,16,22,21,5,18,4,24,13,6,23,9,17,3,20,7 }   // V i
                              }; 

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

/**
 * This function takes care of figuring out which key has been pressed
 * @return int A unique integer for the key pressed
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

  if (kval < 99) {otime = millis();}
  //Starts key debounce timer
  if ((kval >= 0) && (kval <= 99)) {windex = true;}  

  // prints keybord value to serial monitor
  if (DEBUG && kval != kvalo && kval != 100) {Serial.print("readkbde() : kval="); Serial.println(kval);}
  if (DEBUG && kval != kvalo && kval <= 26) {Serial.print("readkbde() : char="); Serial.println(CHARS[kval]);}
  return kval;
}

/**
 * Function to change operating Mode
 */
void modeselect() {
  mode++;
  if (mode >=6) {mode = 0;}
  if (DEBUG) {Serial.print("modeselect() : mode="); Serial.println(mode);}
  windex = false;
}

/**
 * Mode 0
 * Default Mode: Enigma is a typewriter
 */
void mode0() {
  if ((keyval >= 0) && (keyval <= 25)) {
    lampval = keyval;
  } else {
    lampval = 100;
  }
  lampita();
  delay(3);
  lampitb();
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

  lampval = reflect[0]; // Indicate selected reflector B/C in the lamp array
  lampita();
  delay(3);
  lampitb();
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
  digitalWrite(LED5, HIGH);
  boolean printsignalpath = false;
  
  char signalpath[13]; // Used for serial out for studying the internals of the Enigma

  if (keyval >= 26) {  // A function key has been pressed
    lampval = 100;
  } else {             // A character key has been pressed
    if ((keyval >= 0) && (keyval <= 25)) {  
      if (windex) { 
        printsignalpath = true;
        procesvala = keyval;  
        indexwheels();
      }
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
  
  lampita();
  delay(3);
  lampitb();
  
  nixisend();
  
  dig4 = 37;
  dig3 = 37; 
  dig2 = 37; 
  dig1 = 37; 
  
  digitalWrite(LED5, LOW);
}

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

/** 
 *  Send characters to the four sixteen-segment displays
 */
void nixisend() {
  sixteenSegWrite(0, dig1);
  sixteenSegWrite(1, dig2);
  sixteenSegWrite(2, dig3);
  sixteenSegWrite(3, dig4);
}

/**
 * Display the marquee text defined in the MARQUEETEXT[] array on the LED segments
 */             
void marquee() {
  static unsigned long mtime;  
  time = millis();
  if ( mtime < time) {
    mtime = time + 400;
    mdex++;
  }
  dig1 = MARQUEETEXT[behavior][mdex];
  dig2 = MARQUEETEXT[behavior][mdex + 1];
  dig3 = MARQUEETEXT[behavior][mdex + 2];
  dig4 = MARQUEETEXT[behavior][mdex + 3];
  if (mdex >= MARQUEECHARS) {mdex = 0;}
  nixisend();              
}

/**
 * Function that actually turns on each of 17 appropriate segments 
 * on each of the sixteen-segment displays
 * @param int digit
 * @param int character
 */
void sixteenSegWrite(int digit, int character) {
  digitalWrite(ANODEPINS[digit],0);
  for (int index = 0; index < 17; index++) {
    digitalWrite(SEGMENTPINS[index], SEGMENTVALS[character][index]);
  }
  delay(3);
  for (int index = 0; index <= 16; index++) {
    digitalWrite(SEGMENTPINS[index], 1);   
  }
  digitalWrite(ANODEPINS[digit], 1);
}

/**
 * Send the characters D O N E to the LED display, 
 * and reset the dig[1-4] vars to 37.
 */
void done() {
  dig1 = 3; dig2 = 14; dig3 = 13; dig4 = 4;
  nixisend();
  dig1 = 37; dig2 = 37; dig3 = 37; dig4 = 37;
}

/**
 * Check/rotate wheels
 */   
void indexwheels() {
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

/**
 * Read the plugboard settings
 */   
void readplugs() {
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
