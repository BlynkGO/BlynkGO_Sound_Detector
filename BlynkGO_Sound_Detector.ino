#include "BlynkGO_Sound.h"

BlynkGO_Sound sound;

void setup() {
  Serial.begin(115200); Serial.println();
}

void loop() {
  sound.input_data(analogRead(A0));
  if(sound.available()){
    double detected_freq = sound.peak_freq();
    Serial.println(detected_freq);
  }
}
