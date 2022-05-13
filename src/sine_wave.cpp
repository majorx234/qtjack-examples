#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define _USE_MATH_DEFINES

#include "sine_wave.h"

void sine_wave(double values[], int freq, int freq_amp, int num_samples, int  offset, int sample_rate) {
  //int num_samples  = static_cast<int>(duration * sample_rate);
  for (int i =  offset; i < num_samples + offset; i++) {
    values[i-offset] =  sin((2 * M_PI * freq_amp * i)/ sample_rate) * sin((2 * M_PI * freq * i) / sample_rate)  ;
  }
}
