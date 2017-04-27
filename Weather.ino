#include <Wire.h>
#include <SparkFunT5403.h>

/* Si7021 Humidity and Temperature Sensor */
// https://github.com/sparkfun/Si7021_Breakout/tree/master/Libraries/Arduino/Si7021

int polling_interval = 10; //Seconds to wait between polling

float humidity = 0;
float tempf = 0;
Weather tempHumidSensor;

/* Barometer Variables */
// Barometer code reused from example https://github.com/sparkfun/SparkFun_T5403_Barometric_Sensor_Arduino_Library/blob/master/examples/SparkFun_T5403_example/SparkFun_T5403_example.ino
bool barometerConnected = false;
T5403 barometer(MODE_I2C);
float bar_temperature_c, bar_temperature_f;
double pressure_abs, pressure_relative, altitude_delta, pressure_baseline;
double base_altitude = 62; //Altitude in meters

void startWeather() {
  checkForBarometer();

  // Setup Barometer if it is connected
  if (barometerConnected) { 
    //Retrieve calibration constants for conversion math.
    barometer.begin();
    // Grab a baseline pressure for delta altitude calculation.
    pressure_baseline = barometer.getPressure(MODE_ULTRA);
    barometerUpdate();
    timer.setInterval(polling_interval * 1000, barometerUpdate);
  }

  //Setup temperature and humidity sensor
  tempHumidSensor.begin();
  tempHumidUpdate();
  timer.setInterval(polling_interval * 1000, tempHumidUpdate);
  
}

void tempHumidUpdate() {
  humidity = tempHumidSensor.getRH();
  tempf = tempHumidSensor.getTempF();
//  Serial.print("Si7021 temp: ");
//  Serial.print(tempf);
//  Serial.print("F, ");
//
//  Serial.print("humidity: ");
//  Serial.print(humidity);
//  Serial.println("%");
}

void barometerUpdate() {
  // Read temperature from the sensor in deg C. This operation takes about
  // 4.5ms to complete.
  bar_temperature_c = barometer.getTemperature(CELSIUS);

  // Read temperature from the sensor in deg F. This operation takes about
  // 4.5ms to complete. Converting to Fahrenheit is not internal to the sensor.
  // Additional math is done to convert a Celsius reading.
  bar_temperature_f = barometer.getTemperature(FAHRENHEIT);

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

//  // Taking our baseline pressure at the beginning we can find an approximate
//  // change in altitude based on the differences in pressure.
//  altitude_delta = altitude(pressure_abs , pressure_baseline);
//
//  // Report values via UART
//  Serial.print("Temperature C = ");
//  Serial.println(bar_temperature_c / 100);
//
//  Serial.print("Temperature F = ");
//  Serial.println(bar_temperature_f / 100);
//
//  Serial.print("Pressure abs (Pa)= ");
//  Serial.println(pressure_abs);
//
//  Serial.print("Pressure relative (hPa)= ");
//  Serial.println(pressure_relative);
//
//  Serial.print("Altitude change (m) = ");
//  Serial.println(altitude_delta);
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
