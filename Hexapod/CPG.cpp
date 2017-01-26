#include "CPG.h"

/* Oscillator network structure:
(knee oscillators are not represented as each knee oscillator is only linked to its shoulder)
1 -- 2
|    |
3 -- 4
|    |
5 -- 6 */

const float CPG::WEIGHT[N_OSC][N_OSC] =
{
  //1, 2, 3, 4, 5, 6
  { 0, 1, 1, 0, 0, 0 },//1
  { 1, 0, 0, 1, 0, 0 },//2
  { 1, 0, 0, 1, 1, 0 },//3
  { 0, 1, 1, 0, 0, 1 },//4
  { 0, 0, 1, 0, 0, 1 },//5
  { 0, 0, 0, 1, 1, 0 } //6
};

void CPG::setup()
{
  freq = 1.0;
  for (int i = 0; i < N_OSC; ++i)
  {
    phase[i] = random(1000) / 100.0f;
  }
  gait = GAIT_NONE;
  setGait(gait);
}

void CPG::update(float dt)
{
  for (int i = 0; i < N_OSC; ++i)
  {
    float dPhase = freq;
    for (int j = 0; j < N_OSC; ++j)
    {
      dPhase += WEIGHT[i][j] * sin(phase[j] - phase[i] - phaseBias[i][j]);
    }
    newPhase[i] = phase[i] + dt * dPhase;
  }
  for (int i = 0; i < N_OSC; ++i)
  {
    phase[i] = newPhase[i];
  }
}

float CPG::readPhase(int index)
{
  if (index > -1 && index < N_OSC) return phase[index];
  return 0;
}

int CPG::getGait()
{
  return gait;
}

void CPG::setGait(int gait_)
{
  gait = gait_;
  updateGaitPhaseBias();

  for (int i = 0; i < N_OSC + 1; ++i)
  {
    theta[i] *= M_PI;
  }

  phaseBias[0][0] = 0;        phaseBias[0][1] = theta[0]; phaseBias[0][2] = theta[1]; phaseBias[0][3] = 0;        phaseBias[0][4] = 0;        phaseBias[0][5] = 0;
  phaseBias[1][0] = theta[0]; phaseBias[1][1] = 0;        phaseBias[1][2] = 0;        phaseBias[1][3] = theta[2]; phaseBias[1][4] = 0;        phaseBias[1][5] = 0;
  phaseBias[2][0] = theta[1]; phaseBias[2][1] = 0;        phaseBias[2][2] = 0;        phaseBias[2][3] = theta[3]; phaseBias[2][4] = theta[4]; phaseBias[2][5] = 0;
  phaseBias[3][0] = 0;        phaseBias[3][1] = theta[2]; phaseBias[3][2] = theta[3]; phaseBias[3][3] = 0;        phaseBias[3][4] = 0;        phaseBias[3][5] = theta[5];
  phaseBias[4][0] = 0;        phaseBias[4][1] = 0;        phaseBias[4][2] = theta[4]; phaseBias[4][3] = 0;        phaseBias[4][4] = 0;        phaseBias[4][5] = theta[6];
  phaseBias[5][0] = 0;        phaseBias[5][1] = 0;        phaseBias[5][2] = 0;        phaseBias[5][3] = theta[5]; phaseBias[5][4] = theta[6]; phaseBias[5][5] = 0;
}

void CPG::updateGaitPhaseBias()
{
  // 7 elements representing links:
  // (1,2) (1, 3) (2, 4) (3, 4) (3, 5) (4, 6) (5, 6)
  switch (gait)
  {
    case GAIT_METACHRONAL:
      {
        theta[0] = 1.0f;
        theta[1] = 1.0f / 3.0f;
        theta[2] = 1.0f / 3.0f;
        theta[3] = 1.0f;
        theta[4] = 1.0f / 3.0f;
        theta[5] = 1.0f / 3.0f;
        theta[6] = 1.0f;
      }

      break;
    case GAIT_RIPPLE:
      {
        theta[0] = -1.0f;
        theta[1] = -3.0f / 2.0f;
        theta[2] = 1.0f / 2.0f;
        theta[3] = 1.0f;
        theta[4] = 1.0f / 2.0f;
        theta[5] = 1.0f / 2.0f;
        theta[6] = 1.0f;
      };
      break;
    case GAIT_TRIPOD:
      {
        theta[0] = 1.0f;
        theta[1] = 1.0f;
        theta[2] = -1.0f;
        theta[3] = -1.0f;
        theta[4] = -1.0f;
        theta[5] = 1.0f;
        theta[6] = 1.0f;
      };
      break;
    default:
      {
        theta[0] = 0;
        theta[1] = 0;
        theta[2] = 0;
        theta[3] = 0;
        theta[4] = 0;
        theta[5] = 0;
        theta[6] = 0;
      };
  }
}
