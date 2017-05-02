#include <Wire.h>
#include <SparkFunT5403.h>
#include <WiFiClient.h>
#include <Temboo.h>
#include "TembooAccount.h"

/* Si7021 Humidity and Temperature Sensor */
// https://github.com/sparkfun/Si7021_Breakout/tree/master/Libraries/Arduino/Si7021

WiFiClient client;

#define polling_interval 10 //Seconds to wait between polling

float humidity = 0;
float tempf = 0;
Weather tempHumidSensor;

#if INDOOR
float outside_humidity = 0;
float outside_tempf = 0;
float outside_pressure = 0;
#endif

/* Barometer Variables */
// Barometer code reused from example https://github.com/sparkfun/SparkFun_T5403_Barometric_Sensor_Arduino_Library/blob/master/examples/SparkFun_T5403_example/SparkFun_T5403_example.ino
bool barometerConnected = false;
T5403 barometer(MODE_I2C);
float bar_temperature_c, bar_temperature_f;
double pressure_abs, pressure_relative, altitude_delta, pressure_baseline;
double base_altitude = 62; //Altitude in meters

void sendWeatherInfo() {
  tempHumidUpdate();

	#if OUTDOOR
  barometerUpdate();
  bridge.virtualWrite(V10, tempf);
  bridge.virtualWrite(V11, humidity);
  bridge.virtualWrite(V12, pressure_relative);
	#else
  Blynk.virtualWrite(V0, tempf);
  Blynk.virtualWrite(V1, outside_tempf);
  Blynk.virtualWrite(V2, humidity);
  Blynk.virtualWrite(V3, outside_humidity);
  Blynk.virtualWrite(V4, outside_pressure);
	#endif

  logWeather();
}

void startWeather() {
  checkForBarometer();

  // Setup Barometer if it is connected
  if (barometerConnected) {
    //Retrieve calibration constants for conversion math.
    barometer.begin();
    // Grab a baseline pressure for delta altitude calculation.
    pressure_baseline = barometer.getPressure(MODE_ULTRA);
    barometerUpdate();
  }

  //Setup temperature and humidity sensor
  tempHumidSensor.begin();
  tempHumidUpdate();
}

String getWeatherJSON() {
  tempHumidUpdate();
  barometerUpdate();

  String json = "[[\"" + getCurrentTime() + "\", \"" + String(tempf) + "\", \"" + String(humidity) + "\"";
  if (barometerConnected) {
    json = json + ", \"" + String(pressure_relative) + "\"";
  }

  json = json + "]]";
  return json;
}

void printWeatherJSON() {
  Serial.println(getWeatherJSON());
}

void logWeather() {
  String weather = getWeatherJSON();
  Serial.println(weather);
  Serial.println("Making Choreo");
  TembooChoreo AppendValuesChoreo(client);
  AppendValuesChoreo.begin();

  AppendValuesChoreo.setAccountName(TEMBOO_ACCOUNT);
  AppendValuesChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
  AppendValuesChoreo.setAppKey(TEMBOO_APP_KEY);

  AppendValuesChoreo.setProfile("WeatherLog");
  AppendValuesChoreo.addInput("Values", weather);


  AppendValuesChoreo.setChoreo("/Library/Google/Sheets/AppendValues");

  Serial.println("Running Choreo");
  int result = AppendValuesChoreo.run();
  Serial.println("Temboo result: " + String(result));

  while(AppendValuesChoreo.available()) {
    char c = AppendValuesChoreo.read();
    Serial.print(c);
  }

  AppendValuesChoreo.close();
}

void tempHumidUpdate() {
  humidity = tempHumidSensor.getRH();
  tempf = tempHumidSensor.getTempF();
}

void barometerUpdate() {
  // Read pressure from the sensor in Pa. This operation takes about
  // 67ms to complete in ULTRA_MODE.  Other Modes are available for faster, yet
  // less precise measurements.
  // MODE_LOW      = 5 ms
  // MODE_STANDARD = 11 ms
  // MODE_HIGH     = 19 ms
  // MODE_ULTRA    = 67 ms

  pressure_abs  = barometer.getPressure(MODE_ULTRA);

  // Convert abs pressure with the help of altitude into relative pressure
  // This is used in Weather stations.
  pressure_relative = sealevel(pressure_abs, base_altitude);
}

void checkForBarometer() {
  byte b = 119;
  Wire.begin();

  Wire.beginTransmission (b);
  if (Wire.endTransmission () == 0) {
    barometerConnected = true;
    Serial.println();
    Serial.println("Barometer attached");
  }
  delay (5);  // give devices time to recover
}

// Given a pressure P (Pa) taken at a specific altitude (meters),
// return the equivalent pressure (mb) at sea level.
// This produces pressure readings that can be used for weather measurements.
double sealevel(double P, double A) {
  return((P/100)/pow(1-(A/44330.0),5.255));
}


// Given a pressure measurement P (Pa) and the pressure at a baseline P0 (Pa),
// return altitude (meters) above baseline.
double altitude(double P, double P0) {
  return(44330.0*(1-pow(P/P0,1/5.255)));
}

#if INDOOR
BLYNK_WRITE(V10) {
  outside_tempf = param.asFloat();
}

BLYNK_WRITE(V11) {
  outside_humidity = param.asFloat();
}

BLYNK_WRITE(V12) {
  outside_pressure = param.asFloat();
}
#endif

