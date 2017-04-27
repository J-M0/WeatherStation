#include <Temboo.h>
#include <BlynkSimpleEsp8266.h>
#include "BlynkCreds.h"
#include "SparkFun_Si7021_Breakout_Library.h"
#include <Wire.h>
#include <SimpleTimer.h>



#define BLYNK_PRINT Serial

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

