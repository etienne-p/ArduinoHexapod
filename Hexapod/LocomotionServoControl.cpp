#include "LocomotionServoControl.h"

const float LocomotionServoControl::PHASE_OFFSET[N_LEGS_PLUS_ONE] = { 0, PI, 0, PI, 0, PI, 0 };

void LocomotionServoControl::setup()
{
  sig = 1.0f;
  right = .0f;
  left = .0f;
  smoothing = 1.0f;
  shoulderAngleLimMin = -12.0f;
  shoulderAngleLimMax = 12.0f;
  kneeAngleLimMin = -12.0f;
  kneeAngleLimMax = 12.0f;

  memset(shoulderJoints, .0f, sizeof(shoulderJoints));
  memset(kneeJoints, .0f, sizeof(kneeJoints));
  memset(phaseOffset, .0f, sizeof(phaseOffset));
  memset(dPhaseOffset, .0f, sizeof(dPhaseOffset));
  memset(tPhaseOffset, .0f, sizeof(tPhaseOffset));
  memset(amp, .0f, sizeof(amp));
  memset(dAmp, .0f, sizeof(dAmp));
  memset(tAmp, .0f, sizeof(tAmp));

  getKneeJointPhaseOffset(left, right, phaseOffset);
  getKneeJointPhaseOffset(left, right, tPhaseOffset);
  getShoulderJointAmp(left, right, amp);
  getShoulderJointAmp(left, right, tAmp);
}

void LocomotionServoControl::update(float dt)
{
  cpg.update(dt);
  getKneeJointPhaseOffset(left, right, tPhaseOffset);
  getShoulderJointAmp(left, right, tAmp);

  int i = 0;
  for (i = 0; i < N_LEGS; ++i)
  {
    // Critically damped second order differential equation
    // http://mathproofs.blogspot.ca/2013/07/critically-damped-spring-smoothing.html
    float ddPhaseOffset = smoothing * ((smoothing / 4.0) * (tPhaseOffset[i] - phaseOffset[i]) - dPhaseOffset[i]); // 2nd derivative
    dPhaseOffset[i] = dPhaseOffset[i] + dt * ddPhaseOffset; // 1st derivative
    phaseOffset[i] = phaseOffset[i] + dt * dPhaseOffset[i];

    float ddAmp = smoothing * ((smoothing / 4.0) * (tAmp[i] - amp[i]) - dAmp[i]); // 2nd derivative
    dAmp[i] = dAmp[i] + dt * ddAmp; // 1st derivative
    amp[i] = amp[i] + dt * dAmp[i];
  }


  for (i = 0; i < N_LEGS; ++i)
  {
    float osc = sin(cpg.readPhase(i) + PHASE_OFFSET[i]);

    osc = (1.0f / (1.0f + exp(-sig * osc)) - .5f) * 2.0f;

    shoulderJoints[i] = Util::lerp(shoulderAngleLimMin, shoulderAngleLimMax, .5f * (osc + 1.0f)) * amp[i];
  }

  // legs stops moving if the shoulder does
  float a = 1.0f - exp(-4.0f * Util::clamp01(max(abs(left), abs(right))));

  for (i = 0; i < N_LEGS; ++i)
  {
    float osc = sin(cpg.readPhase(i) * 1.0 + phaseOffset[i] * PI / 2.0f);
      
    osc = (1.0f / (1.0f + exp(-sig * osc)) - .5) * 2.0f; // sigmoid

    kneeJoints[i] = Util::lerp(kneeAngleLimMin, kneeAngleLimMax, .5f * (osc + 1.0f)) * a;
  }
}

float LocomotionServoControl::readShoulderJoint(int index)
{
  if (index > -1 && index < N_LEGS) return shoulderJoints[index];
  return 0;
}

float LocomotionServoControl::readKneeJoint(int index)
{
  if (index > -1 && index < N_LEGS) return kneeJoints[index];
  return 0;
}

void LocomotionServoControl::setGait(int gait)
{
  cpg.setGait(gait);
}

void LocomotionServoControl::setFreq(float freq)
{
  cpg.freq = freq;
}


int LocomotionServoControl::getGait()
{
  return cpg.getGait();
}

void LocomotionServoControl::getKneeJointPhaseOffset(float leftTrack, float rightTrack, float* offset)
{
  for (int i = 0; i < N_LEGS; ++i)
  {
    offset[i] = (i % 2 == 0 ? leftTrack : rightTrack);
  }
}

void LocomotionServoControl::getShoulderJointAmp(float leftTrack, float rightTrack, float* amp)
{
  for (int i = 0; i < N_LEGS; ++i)
  {
    amp[i] = abs(i % 2 == 0 ? leftTrack : rightTrack);
  }
}

