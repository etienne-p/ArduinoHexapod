// Turns out the order the includes matters, a lot ;)
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include <Hash.h>
#include <functional>
#include "Auth.h" // in this header are defined the Wifi network SSID and password
#include "Consts.h"
#include "Util.h"

class SocketSettings
{
  public:
    bool configChanged;
    bool steeringChanged;
    void setup();
    void loop();
    int getGait();
    float getSig();
    float getSmoothing();
    float getShoulderAngleLimMin();
    float getShoulderAngleLimMax();
    float getKneeAngleLimMin();
    float getKneeAngleLimMax();
    float getServoCalibration(int i);
    float getFreq();
  private:
    int gait; //NONE = 0, METACHRONAL = 1, RIPPLE = 2, TRIPOD = 3 };
    float sig;
    float smoothing;
    float shoulderAngleLimMin;
    float shoulderAngleLimMax;
    float kneeAngleLimMin;
    float kneeAngleLimMax;
    float servoCalibration[N_SERVOS];
    float freq;
    void webSocketEvent(WStype_t type, uint8_t* payload, size_t length);
    ESP8266WiFiMulti WiFiMulti;
    WebSocketsClient webSocket;
};
