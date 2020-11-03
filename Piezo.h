class Piezo_Drum
{
  
  private:
    int channel;  // General MIDI: channel 10 = percussion sounds
    int note;     // General MIDI: note 38 = acoustic snare
    int led_pin;  // LED output
    float fade = 0; 
    
    
    const int thresholdMin = 400;  // minimum reading, avoid noise and false starts
    const unsigned int peakTrackMillis = 5;
    const unsigned int aftershockMillis = 15; // aftershocks & vibration reject
    int state;  // 0=idle, 1=looking for peak, 2=ignore aftershocks
    int peak;   // remember the highest reading
    elapsedMillis msec; // timer to end states 1 and 2

  public:
   
    Piezo_Drum(int _channel, int _note, int _led_pin){
        this->channel = _channel;
        this->note = _note;      
        this->led_pin = _led_pin;
        Serial.println("Loaded Piezo Sensor..." );
    }
  void fadeLed(){
    fade-=0.1;
    if (fade <= 0) {
      fade = 0;
    }
    analogWrite(led_pin, fade);
  }
  void peakDetect(int voltage) {
    
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
          
          fade = 255; //reset LED to full power on hit

          usbMIDI.sendNoteOn(note, velocity, channel);
          //Serial.println("MIDI Note:" + note);
          digitalWrite(LED_BUILTIN, HIGH);
           

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
          digitalWrite(LED_BUILTIN, LOW);
          state = 0; // go back to idle when
        }
    }
  }
  

};
