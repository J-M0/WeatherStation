#include <Temboo.h>
#include <BlynkSimpleEsp8266.h>
#include "BlynkCreds.h"

void setup() {
  startNTP();
  startWiFi();
  Blynk.config(BLYNK_AUTH);
}

void loop() {
  // put your main code here, to run repeatedly:

}
