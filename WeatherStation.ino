#include <Temboo.h>
#include <BlynkSimpleEsp8266.h>
#include "BlynkCreds.h"
#include "SparkFun_Si7021_Breakout_Library.h"
#include <Wire.h>
#include <SimpleTimer.h>
#include <Ticker.h>

float humidity = 0;
float tempf = 0;

SimpleTimer timer;

Weather sensor;

void setup() {
  Serial.begin(9600);
//  startWiFi();
//  startNTP();
//  Blynk.config(BLYNK_AUTH);
  sensor.begin();
  startWeather();
  updateIndoor();
  timer.setInterval(5 * 1000, updateIndoor);
}

void loop() {
  Blynk.run();
  timer.run();
}

void updateIndoor() {
  tempf = sensor.getTempF();
  humidity = sensor.getRH();

  Serial.print("Indoor temp: ");
  Serial.print(tempf);
  Serial.print("F, ");

  Serial.print("Indoor humidity: ");
  Serial.print(humidity);
  Serial.println("%");
  Blynk.virtualWrite(V1, tempf);
}

void updateOutdoor() {
  tempf = sensor.getTempF();
  humidity = sensor.getRH();

  Serial.print("Outdoor temp: ");
  Serial.print(tempf);
  Serial.print("F, ");

  Serial.print("Outdoor humidity: ");
  Serial.print(humidity);
  Serial.println("%");
  Blynk.virtualWrite(V0, tempf);
}
