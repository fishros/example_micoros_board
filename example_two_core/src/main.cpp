#include <Arduino.h>

void Task1code(void *parameter)
{
  pinMode(2,OUTPUT);
  pinMode(4,OUTPUT);
  for (;;)
  { // infinite loop
    Serial.print("Task1 running on core ");
    Serial.println(xPortGetCoreID());
    digitalWrite(2, HIGH);
    digitalWrite(4, HIGH);
    delay(500);
    digitalWrite(2, LOW);
    digitalWrite(4, HIGH);
    delay(500);
  }
}

void Task2code(void *parameter)
{
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());
  for (;;)
  { // infinite loop
    digitalWrite(2, HIGH);
    delay(500);
    digitalWrite(2, LOW);
    delay(500);
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.print("setup() function running on core: ");
  Serial.println(xPortGetCoreID());
  xTaskCreatePinnedToCore(Task1code, "Task1", 10000, NULL, 1, NULL,  0);
}
void loop()
{
  Serial.print("loop() function running on core: ");
  Serial.println(xPortGetCoreID());
  delay(500);
}