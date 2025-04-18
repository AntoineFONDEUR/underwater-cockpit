#include <Arduino.h>
#include <FastLED.h>
#include <Joystick.h>

#define LED_PIN     16
#define NUM_LEDS    1

int incomingByte = 0;
bool state = false;

CRGB leds[NUM_LEDS];

void setup() {

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  Serial.begin(9600);
  Joystick.begin();

}

void loop() {

  Joystick.button(5, (bool)BOOTSEL);

  if (Serial.available() > 0) {

    incomingByte = Serial.read();

    if (incomingByte == 'h'){
      leds[0] = CRGB(255, 255, 255);
      FastLED.show();
    }

    if (incomingByte == 'l'){
      leds[0] = CRGB(0, 0, 0);
      FastLED.show();
    }

  }

  delay(10);
}
