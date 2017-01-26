#include <RCSwitch.h>

#define JOY_PIN_X 0
#define JOY_PIN_Y 1

RCSwitch mySwitch = RCSwitch();

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  mySwitch.enableTransmit(2);
  yield();
}

void loop()
{
  digitalWrite(LED_BUILTIN, HIGH);
  // we'll pack both readings in a 16 bit value, allocating 8 bit per reading
  uint16_t axisX = (analogRead(JOY_PIN_X) / 1024.f) * 255.f;
  uint16_t axisY = (analogRead(JOY_PIN_Y) / 1024.f) * 255.f;
  uint16_t packed = axisX + (axisY << 8);
  if (Serial)
  {
    Serial.print("x: ");
    Serial.print(axisX);
    Serial.print(" y: ");
    Serial.print(axisY);
    Serial.print(" packed: ");
    Serial.println(packed);
  }
  mySwitch.send(packed, 32);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
}
