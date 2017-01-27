#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <WebSockets.h>
#include <WebSocketsClient.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <RCSwitch.h>
#include <Hash.h>
#include <functional>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include "Consts.h"
#include "CPG.h"
#include "LocomotionServoControl.h"
#include "SocketSettings.h"
#include "SteeringControl.h"

unsigned long timeStamp;

LocomotionServoControl locomotionControl;
SocketSettings settings;
Adafruit_PWMServoDriver pwm;
SteeringControl steeringControl;

void setup()
{
  timeStamp = millis();
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(50);  // see tower pro 90 spec
  steeringControl.setup();
  settings.setup();
  yield();
}

void updateSettings()
{
  settings.loop();

  if (settings.configChanged)
  {
    settings.configChanged = false;

    locomotionControl.setGait(settings.getGait());
    locomotionControl.setFreq(settings.getFreq());
    locomotionControl.sig = settings.getSig();
    locomotionControl.smoothing = settings.getSmoothing();
    locomotionControl.shoulderAngleLimMin = settings.getShoulderAngleLimMin();
    locomotionControl.shoulderAngleLimMax = settings.getShoulderAngleLimMax();
    locomotionControl.kneeAngleLimMin = settings.getKneeAngleLimMin();
    locomotionControl.kneeAngleLimMax = settings.getKneeAngleLimMax();
  }
}

void updateSteering()
{
  steeringControl.update();
  locomotionControl.left = steeringControl.getLeft();
  locomotionControl.right = steeringControl.getRight();
}

void updateLocomotion()
{
  unsigned long newTimeStamp = millis();
  locomotionControl.update((float)(newTimeStamp - timeStamp) / 1000.0);
  timeStamp = newTimeStamp;
}

void updateServos()
{
  for (int i = 0; i < N_LEGS; ++i)
  {
    // Here we combine the locomotion control computed servo angles
    // with the calibration values (as the bot needs calibration to compensate for mechanical imprecisions)
    float shoulderAngle = settings.getServoCalibration(i) + locomotionControl.readShoulderJoint(i);
    float kneeAngle = settings.getServoCalibration(N_LEGS + i) + locomotionControl.readKneeJoint(i);

    pwm.setPWM(i,  0, SERVO_MIN + ((SERVO_MAX - SERVO_MIN) / 2.0f) * (1.0f + shoulderAngle / 90.0f));
    pwm.setPWM(N_LEGS + i, 0, SERVO_MIN + ((SERVO_MAX - SERVO_MIN) / 2.0f) * (1.0f + kneeAngle / 90.0f));
  }
}

void loop()
{
   updateSettings();
   updateSteering();
   updateLocomotion();
   updateServos();
   delay(50);
}


