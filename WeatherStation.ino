#include <BlynkSimpleEsp8266.h>
#include "BlynkCreds.h"
#include "SparkFun_Si7021_Breakout_Library.h"
#include <Wire.h>
#include <SimpleTimer.h> //playground.arduino.cc/Code/SimpleTimer

SimpleTimer timer;

#if OUTDOOR
WidgetBridge bridge(V0);
#endif

void setup() {
  Serial.begin(9600);
  startWiFi();
  startNTP();
  Blynk.config(BLYNK_AUTH);
  startWeather();
  timer.setInterval(10 * 1000, printWeatherJSON);
  timer.setInterval(10 * 1000, sendWeatherInfo);
//  timer.setInterval(10 * 1000, logWeather);
}

void loop() {
  Blynk.run();
  timer.run();
}

#if OUTDOOR
BLYNK_CONNECTED() {
  bridge.setAuthToken(INDOOR_AUTH);
}
#endif

