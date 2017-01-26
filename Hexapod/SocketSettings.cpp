#include "SocketSettings.h"

void SocketSettings::setup()
{
  configChanged = false;
  steeringChanged = false;

  // Here SSID and PWD are defined in an external header file,
  // Provide your own credentials
  WiFiMulti.addAP(MY_SSID, MY_PWD);

  while (WiFiMulti.run() != WL_CONNECTED)
  {
    delay(100);
  }
  webSocket.begin(SERVER_IP, 3000);
  webSocket.onEvent(std::bind(&SocketSettings::webSocketEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void SocketSettings::loop()
{
  webSocket.loop();
}

void SocketSettings::webSocketEvent(WStype_t type, uint8_t* payload, size_t length)
{
  switch (type)
  {
    case WStype_DISCONNECTED:
      if (Serial) Serial.printf("[WSc] Disconnected!\n");
      break;
    case WStype_CONNECTED:
      {
        if (Serial) Serial.printf("[WSc] Connected to url: %s\n",  payload);
        webSocket.sendTXT("Connected");
      }
      break;
    case WStype_TEXT:
      {
        //Serial.printf("[WSc] get text: %s\n", payload);
        StaticJsonBuffer<JSON_BUFFER_SIZE> jsonBuffer;
        JsonObject& root = jsonBuffer.parseObject((char*)payload);
        if (!root.success())
        {
          if (Serial) Serial.println("[WSc] failed to parse payload as json");
          return;
        }

        // parse settings
        gait = root["gait"]; //NONE = 0, METACHRONAL = 1, RIPPLE = 2, TRIPOD = 3 };
        sig = root["sig"];
        freq = root["freq"];
        smoothing = root["smoothing"];
        shoulderAngleLimMin = root["shoulderAngleLimMin"];
        shoulderAngleLimMax = root["shoulderAngleLimMax"];
        kneeAngleLimMin = root["kneeAngleLimMin"];
        kneeAngleLimMax = root["kneeAngleLimMax"];
        for (int i = 0; i < N_SERVOS; ++i)
        {
          servoCalibration[i] = root["servoCalibration"][i];
        }
        configChanged = true;

      }
      break;
  }
}

int SocketSettings::getGait()
{
  return gait;
}

float SocketSettings::getSig()
{
  return sig;
}

float SocketSettings::getFreq()
{
  return freq;
}

float SocketSettings::getSmoothing()
{
  return smoothing;
}

float SocketSettings::getShoulderAngleLimMin()
{
  return shoulderAngleLimMin;
}

float SocketSettings::getShoulderAngleLimMax()
{
  return shoulderAngleLimMax;
}

float SocketSettings::getKneeAngleLimMin()
{
  return kneeAngleLimMin;
}

float SocketSettings::getKneeAngleLimMax()
{
  return kneeAngleLimMax;
}

float SocketSettings::getServoCalibration(int i)
{
  if (i < 0 || i > N_SERVOS - 1)
  {
    return 0;
  }
  return servoCalibration[i];
}
