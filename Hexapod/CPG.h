#ifndef CPG_H
#define CPG_H

#include <math.h>
#include "arduino.h"
#include "Consts.h"

class CPG
{
  public:

    float readPhase(int index);
    void setup();
    void setGait(int gait_);
    int getGait();
    void update(float dt);
    float freq; // oscillator natural frequency
  private:
    int gait;
    float theta[N_OSC_PLUS_ONE];
    float phase[N_OSC];
    float newPhase[N_OSC];
    static const float WEIGHT[N_OSC][N_OSC];
    void updateGaitPhaseBias();
    float phaseBias[N_OSC][N_OSC] ; // phase bias between oscillators in the kuramoto model
};

#endif // CPG
