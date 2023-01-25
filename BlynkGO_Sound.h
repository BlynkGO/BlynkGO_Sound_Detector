#ifndef __BLYNKGO_SOUND_H__
#define __BLYNKGO_SOUND_H__

#include <Arduino.h>
#include "src/arduinoFFT.h"

#define SOUND_MAX_SAMPLES     128

class BlynkGO_Sound {
  public:
    BlynkGO_Sound(float sampling_freq=2048) {
      this->_sampling_freq    = sampling_freq;
      this->_sampling_period  = round(1000000*(1.0/this->_sampling_freq)); //Period in microseconds }
    }

    void input_data(int analogread_value){
      static unsigned long _timer_us;
      if( micros() >= _timer_us ){
        this->vReal[this->index_sample] = analogread_value;
        this->vImag[this->index_sample] = 0;

        this->index_sample++;
        if( this->index_sample >= SOUND_MAX_SAMPLES ){
          /*Perform FFT on samples*/
          _FFT.DCRemoval();
          _FFT.Windowing(this->vReal, SOUND_MAX_SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
          _FFT.Compute(this->vReal, vImag, SOUND_MAX_SAMPLES, FFT_FORWARD);
          _FFT.ComplexToMagnitude(this->vReal, this->vImag, SOUND_MAX_SAMPLES);
          /*Find peak frequency and print peak*/
          this->_peak = _FFT.MajorPeak(this->vReal, SOUND_MAX_SAMPLES, this->_sampling_freq);
          this->index_sample = 0;
          this->_available =true;
        }
        _timer_us = micros() + this->_sampling_period;
      }
    }

    inline bool available()     { return this->_available; }
    double peak_freq()               { 
      double _p = this->_peak;
      this->_peak = 0;
      this->_available = false;
      return _p;
    }

    double vReal[SOUND_MAX_SAMPLES]; //create vector of size SAMPLES to hold real values
    double vImag[SOUND_MAX_SAMPLES]; //create vector of size SAMPLES to hold imaginary values

  private:
    arduinoFFT _FFT = arduinoFFT();
    float _sampling_freq;
    float _sampling_period;
    int   index_sample =0;
    bool  _available =false;
    double _peak;
};

#endif //__BLYNKGO_SOUND_H__
