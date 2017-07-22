/*
  Capacitive-Touch Arduino Keyboard Piano
  
  Plays piano tones through a buzzer when the user taps touch-sensitive piano "keys"
  
  Created  18 May 2013
  Modified 23 May 2013
  by Tyler Crumpton and Nicholas Jones
  
  This code is released to the public domain. For information about the circuit,
  visit the Instructable tutorial at http://www.instructables.com/id/Capacitive-Touch-Arduino-Keyboard-Piano/
*/

#include <CapacitiveSensor.h>
//#include "pitches.h"
#include <MozziGuts.h>
#include <Oscil.h> // oscillator template
#include <tables/sin2048_int8.h> // sine table for oscillator
#include <AutoMap.h>
#include <RollingAverage.h>
#include <CapacitiveSensor.h>

// use: Oscil <table_size, update_rate> oscilName (wavetable), look in .h file of table #included above
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);

#define COMMON_PIN      2    // The common 'send' pin for all keys
//#define BUZZER_PIN      A4   // The output pin for the piezo buzzer
#define NUM_OF_SAMPLES  1   // Higher number whens more delay but more consistent readings
//#define CAP_THRESHOLD   150  // Capactive reading that triggers a note (adjust to fit your needs)
//#define NUM_OF_KEYS     8    // Number of keys that are on the keyboard

// This macro creates a capacitance "key" sensor object for each key on the piano keyboard:
#define CS(Y) CapacitiveSensor(2, Y)

// Each key corresponds to a note, which are defined here. Uncomment the scale that you want to use:
//int notes[]={NOTE_C4,NOTE_D4,NOTE_E4,NOTE_F4,NOTE_G4,NOTE_A4,NOTE_B4,NOTE_C5}; // C-Major scale
//int notes[]={NOTE_A4,NOTE_B4,NOTE_C5,NOTE_D5,NOTE_E5,NOTE_F5,NOTE_G5,NOTE_A5}; // A-Minor scale
//int notes[]={NOTE_C4,NOTE_DS4,NOTE_F4,NOTE_FS4,NOTE_G4,NOTE_AS4,NOTE_C5,NOTE_DS5}; // C Blues scale

// Defines the pins that the keys are connected to:
//CapacitiveSensor keys[] = {CS(3), CS(4)};//, CS(5), CS(6), CS(7), CS(8), CS(9), CS(10)};

CapacitiveSensor   cs_3_4 = CapacitiveSensor(2,3); 
CapacitiveSensor   cs_5_6 = CapacitiveSensor(2,4);


// use #define for CONTROL_RATE, not a constant
#define CONTROL_RATE 64 // powers of 2 please

const int KNOB_PIN = 0; 

const int MIN_IN = 0;
const int MAX_IN = 1023;

const int MIN_C = 0;
const int MAX_C = 400;

//const int MIN_C = -2000;
//const int MAX_C = 1700;

const int MIN_F = 600;
const int MAX_F = 200;

AutoMap kMapF(MIN_IN,MAX_IN,MIN_F,MAX_F);
AutoMap kMapC(MIN_C,MAX_C,MIN_F,MAX_F);

RollingAverage <int, 16> FAverage; 
RollingAverage <int, 64> CapAverage; 
RollingAverage <int, 64> Cap2Average; 

void setup() { 
  startMozzi(CONTROL_RATE); // set a control rate of 64 (powers of 2 please)
  Serial.begin(115200);
  // Turn off autocalibrate on all channels:
 // for(int i=0; i<2; ++i) {
  //  keys[i].set_CS_AutocaL_Millis(0xFFFFFFFF);
    cs_3_4.set_CS_AutocaL_Millis(0xFFFFFFFF);
cs_5_6.set_CS_AutocaL_Millis(0xFFFFFFFF);
 // }
  // Set the buzzer as an output:
 // pinMode(BUZZER_PIN, OUTPUT); 
}

void loop() {    
  // Loop through each key:
  audioHook(); // required here

  
  //for (int i = 0; i < 8; ++i) {
  //  for (int i = 0; i < 2; ++i) {
    // If the capacitance reading is greater than the threshold, play a note:
//    if(keys[i].capacitiveSensor(NUM_OF_SAMPLES) > CAP_THRESHOLD) {
//      tone(BUZZER_PIN, notes[i]); // Plays the note corresponding to the key pressed
  //  Serial.print(keys[0].capacitiveSensor(NUM_OF_SAMPLES));
//    Serial.print("    ");
//    
//    Serial.print(keys[1].capacitiveSensor(NUM_OF_SAMPLES));
//    Serial.print("    ");

//
//        Serial.print(cs_3_4.capacitiveSensor(1));
//    Serial.print("    ");
//    
//    Serial.print(cs_5_6.capacitiveSensor(1));
//    Serial.print("    ");
//
//    Serial.println("    ");
    
//    Serial.print(keys[1].capacitiveSensor(NUM_OF_SAMPLES));
//    Serial.println("    ");
    
//    if (i=2)
//    {
//      Serial.println("");
//    }
    
  //  }
   
  }

  
void updateControl(){
  
int fundamental = mozziAnalogRead(KNOB_PIN)+1;
//fundamental = FAverage.next(fundamental);
fundamental = kMapF(fundamental);
//Serial.print(fundamental);
//Serial.print("  ");

long capsense1_ =  cs_3_4.capacitiveSensor(1);
int capsense1 = (int) capsense1_;
capsense1 = CapAverage.next(capsense1);
//capsense1 = kMapC(capsense1);

long capsense2_ =  cs_5_6.capacitiveSensor(1);
int capsense2 = (int) capsense2_;
capsense2 = Cap2Average.next(capsense2);
//capsense2 = kMapC(capsense2);


//Serial.print(keys[0].capacitiveSensor(NUM_OF_SAMPLES));
//    Serial.print("    ");
//    
//    Serial.print(keys[1].capacitiveSensor(NUM_OF_SAMPLES));
//    Serial.print("    ");

cs_3_4.set_CS_AutocaL_Millis(0xFFFFFFFF);
cs_5_6.set_CS_AutocaL_Millis(0xFFFFFFFF);

Serial.print(capsense1);
Serial.print("  ");

Serial.print(capsense2);
Serial.print("  ");


    Serial.println("    ");

//Serial.print(capsense1_);
//Serial.print("  ");
//Serial.print(capsense1_);
//Serial.print("  ");
//Serial.print(capsense1);
//Serial.print("  ");
//
//Serial.print(capsense2);
//Serial.print("  ");
  //
  //aSin.setFreq(fundamental); // set the frequency
  aSin.setFreq(capsense2); // set the frequency
//    float R = aSin.next();
//    Serial.print(R);
//    Serial.print("  ");
    
    //Serial.println("  ");
}


int updateAudio(){
  return aSin.next(); // return an int signal centred around 0
}

