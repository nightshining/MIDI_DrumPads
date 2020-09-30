class Piezo_Drum
{
  
  private:
    int channel;  // General MIDI: channel 10 = percussion sounds
    int note;     // General MIDI: note 38 = acoustic snare
    
    const int thresholdMin = 60;  // minimum reading, avoid noise and false starts
    const unsigned int peakTrackMillis = 12;
    const unsigned int aftershockMillis = 25; // aftershocks & vibration reject
    int state;  // 0=idle, 1=looking for peak, 2=ignore aftershocks
    int peak;   // remember the highest reading
    elapsedMillis msec; // timer to end states 1 and 2


  public:
   
    Piezo_Drum(int _channel, int _note){
        this->channel = _channel;
        this->note = _note;      
        Serial.println("Loaded Piezo Sensor..." );
    }
   
  void peakDetect(int voltage) {
//    // "static" variables keep their numbers between each run of this function
//    static int state;  // 0=idle, 1=looking for peak, 2=ignore aftershocks
//    static int peak;   // remember the highest reading
//    static elapsedMillis msec; // timer to end states 1 and 2
   

    switch (state) {
      // IDLE state: wait for any reading is above threshold.  Do not set
      // the threshold too low.  You don't want to be too sensitive to slight
      // vibration.
      case 0:
        if (voltage > thresholdMin) {
          //Serial.print("begin peak track ");
          //Serial.println(voltage);
          peak = voltage;
          msec = 0;
          state = 1;
        }
        return;
  
      // Peak Tracking state: capture largest reading
      case 1:
        if (voltage > peak) {
          peak = voltage;  
        }
        if (msec >= peakTrackMillis) {
          //Serial.print("peak = ");
          //Serial.println(peak);
          int velocity = map(peak, thresholdMin, 1023, 1, 127);
          usbMIDI.sendNoteOn(note, velocity, channel);
          Serial.println("Sending MIDI");

          msec = 0;
          state = 2;
        }
        return;
  
      // Ignore Aftershock state: wait for things to be quiet again.
      default:
        if (voltage > thresholdMin) {
          msec = 0; // keep resetting timer if above threshold
        } else if (msec > aftershockMillis) {
          usbMIDI.sendNoteOff(note, 0, channel);
          state = 0; // go back to idle when
        }
    }
  }


};
