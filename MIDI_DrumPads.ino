
#define NUM_SENSORS 4

#include "Piezo.h"

//const int analogPin = A0;
//Piezo_Drum *pad = Piezo_Drum(10, 38);
static const uint8_t analogPins[] = {A0,A1,A2,A3};
Piezo_Drum *pad[NUM_SENSORS];
int state;  // 0=idle, 1=looking for peak, 2=ignore aftershocks
int peak;   // remember the highest reading
elapsedMillis msec; // timer to end states 1 and 2
    

void setup() {
  
  Serial.begin(115200);
  while (!Serial && millis() < 2500) /* wait for serial monitor */ ;
  Serial.println("Start...");
  
  int midiNote = 59; 
  for (int i = 0; i < NUM_SENSORS; i++) {
    int midiCh = 0; 
    midiNote++;
    pad[i] = new Piezo_Drum(midiCh,midiNote);
  }

}

void loop() {
        

      for (int i = 0; i < NUM_SENSORS; i++) {
        //int piezo = analogRead(analogPins[i]);
        pad[i]->peakDetect(analogRead(analogPins[i]));
      }
       // pad[0]->peakDetect(analogRead(analogPins[0]));



  // Serial.println(analogRead(analogPins[0])); DEBUG Pins 

  // MIDI Controllers should discard incoming MIDI messages.
  // http://forum.pjrc.com/threads/24179-Teensy-3-Ableton-Analog-CC-causes-midi-crash
  while (usbMIDI.read()) {
    // ignore incoming messages
  }
}
