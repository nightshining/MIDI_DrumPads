
#define NUM_SENSORS 4

#include "Piezo.h"

//const int analogPin = A0;
//Piezo_Drum *pad = Piezo_Drum(10, 38);
static const uint8_t analogPins[] = {A0,A1,A2,A3}; // Piezo Inputs
static const uint8_t outputPins[] = {3,4,5,6}; // LED outputs

Piezo_Drum *pad[NUM_SENSORS];
int state;  // 0=idle, 1=looking for peak, 2=ignore aftershocks
int peak;   // remember the highest reading
elapsedMillis msec; // timer to end states 1 and 2

// constants won't change. Used here to set a pin number:
const int ledPin =  LED_BUILTIN;// the number of the LED pin

// Variables will change:
int ledState = LOW;             // ledState used to set the LED

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 1000;           // interval at which to blink (milliseconds)

    

void setup() {
  
  Serial.begin(115200);
  while (!Serial && millis() < 2500) /* wait for serial monitor */ ;
  Serial.println("Start...");
   
   pinMode(LED_BUILTIN, OUTPUT); //onboard LED light every hit for debug

  int midiNote = 59; 
  for (int i = 0; i < NUM_SENSORS; i++) {
    int midiCh = 0; 
    midiNote++;
    pinMode(outputPins[i], OUTPUT);
    pad[i] = new Piezo_Drum(midiCh,midiNote, outputPins[i]);
    
  }

}

void loop() {
       
      for (int i = 0; i < NUM_SENSORS; i++) {
        //int piezo = analogRead(analogPins[i]);
        pad[i]->peakDetect(analogRead(analogPins[i]));
        pad[i]->fadeLed();
      }
//        pad[0]->peakDetect(analogRead(analogPins[0]));
//        Serial.println(analogRead(analogPins[0]));

 // Serial.println(analogRead(analogPins[0])); DEBUG Pins 

  // MIDI Controllers should discard incoming MIDI messages.
  // http://forum.pjrc.com/threads/24179-Teensy-3-Ableton-Analog-CC-causes-midi-crash
  while (usbMIDI.read()) {
    // ignore incoming messages
  }
}
