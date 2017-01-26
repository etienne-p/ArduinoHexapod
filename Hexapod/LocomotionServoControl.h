#ifndef LOCOMOTION_SERVO_CONTROL_H
#define LOCOMOTION_SERVO_CONTROL_H


#include <arduino.h>
#include <string.h>

#include "Util.h"
#include "CPG.h"
#include "Consts.h"

class LocomotionServoControl
{
  public:
    float sig; // sigmoid shape
    float right; // think of a tank, moves by changing its tracks speed independently
    float left;
    float smoothing; // smoothing on gait change
    float shoulderAngleLimMin;
    float shoulderAngleLimMax;
    float kneeAngleLimMin;
    float kneeAngleLimMax;
    void setup();
    void update(float dt);
    float readShoulderJoint(int index);
    float readKneeJoint(int index);
    void setGait(int gait);
    void setFreq(float freq);
    int getGait();
  private:
    CPG cpg;
    float shoulderJoints[N_LEGS];
    float kneeJoints[N_LEGS];
    float phaseOffset[N_LEGS]; // Knee joint phase offset
    float dPhaseOffset[N_LEGS]; // Knee joint phase offset derivative
    float tPhaseOffset[N_LEGS]; // Target knee joint phase offset derivative
    float amp[N_LEGS]; // Shoulder joint amp
    float dAmp[N_LEGS]; // Shoulder joint amp derivative
    float tAmp[N_LEGS]; // Target shoulder joint amp
    static const float PHASE_OFFSET[N_LEGS_PLUS_ONE];
    static void getKneeJointPhaseOffset(float leftTrack, float rightTrack, float* offset);
    static void getShoulderJointAmp(float leftTrack, float rightTrack, float* amp);
};

#endif // LOCOMOTION_SERVO_CONTROL_H
