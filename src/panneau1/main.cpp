#include <Arduino.h>
#include <ADS1X15.h>
#include <Joystick.h>
#include <Pannel.h>

ADS1115 ADS(0x48);

Panel myPanel {
  {
    new AxisInput("slider", Pin{29}, 0, 13, 1023, true),
    new AxisInput("right_joystick_h", Pin{2, ADS}, 1, 0, 32767),
    new AxisInput("right_joystick_v", Pin{3, ADS}, 2, 0, 32767),
  },
  {
    new Led("led-intensity", 26)
  },
  ADSSettings{1,ADS}
};

void setup() {
  Serial.begin(9600);
  myPanel.begin();
  pinMode(6, OUTPUT);
}

void loop() {
  digitalWrite(6, LOW);

  // Inputs
  myPanel.read_states();
  myPanel.send_states();

  // Ouputs
  myPanel.receive_target_states();
  myPanel.write_target_states();

}
