#include <BlynkSimpleEsp8266.h>
#include "BlynkCreds.h"
#include "SparkFun_Si7021_Breakout_Library.h"
#include <Wire.h>
#include <SimpleTimer.h> //playground.arduino.cc/Code/SimpleTimer
#include <Ticker.h>

SimpleTimer timer;
Ticker secondTicker;

#if OUTDOOR
WidgetBridge bridge(V0);
#endif

volatile bool tembooWorking = false;
volatile int tembooSeconds = 0;

void watchDog() {
  if(tembooWorking == true) {
    tembooSeconds++;
    if(tembooSeconds == 20) {
      Serial.println("Temboo timed out. Restarting.");
      ESP.restart();
    }
  }
}

void setup() {
  Serial.begin(9600);
  startWiFi();
  startNTP();
  Blynk.config(BLYNK_AUTH);
  startWeather();

  secondTicker.attach(1, watchDog);
  timer.setInterval(10 * 1000, sendWeatherInfo);
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

