#include <Arduino.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Board (from flag) n°");
  Serial.print(BOARD_NUM);
  Serial.print("\tBoard (from specific src) n°");
  Serial.println(1);
}
