#include <Arduino.h>

void setup() {
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  pinMode(34, INPUT);
  //set the resolution to 12 bits (0-4096)
  analogReadResolution(12);
  analogSetAttenuation(ADC_2_5db);
}

void loop() {
  // read the analog / millivolts value for pin 2:
  int analogValue = analogRead(34);
  int analogVolts = analogReadMilliVolts(34);
  
  // print out the values you read:
  Serial.printf("ADC analog value = %d\n",analogValue);
  Serial.printf("ADC millivolts value = %d\n",analogVolts);
  
  delay(100);  // delay in between reads for clear read from serial
}