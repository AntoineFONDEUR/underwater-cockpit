#include <Arduino.h>
#include <ADS1X15.h>
#include <Joystick.h>
#include <Pannel.h>

ADS1115 ADS1(0x48);

const float DEAD_ZONE = 0.05;

Panel myPanel {
  {
    new AxisInput("lights3", Pin{28}, 0, DEAD_ZONE, true , 2, 1023, true),
    new AxisInput("lights2", Pin{27}, 1, DEAD_ZONE, true , 2, 1023, true),
    new AxisInput("lights1", Pin{26}, 2, DEAD_ZONE, true , 2, 1023, true),
    new AxisInput("joystick_tilt", Pin{0, ADS1}, 3, DEAD_ZONE, false, 0, 29300),
    new AxisInput("joystick_v", Pin{2, ADS1}, 4, DEAD_ZONE, false, 0, 29300, true),
    new AxisInput("slider", Pin{3, ADS1}, 5, DEAD_ZONE, true, 0, 29200)
  },
  {
  }
};

void setup() {
  Serial.begin(115200);
  myPanel.begin();

  Wire.begin();
  ADS1.begin();
  ADS1.setGain(1);
}

void loop() {
  // Inputs
  myPanel.read_states();
  myPanel.send_states();

  // Ouputs
  myPanel.receive_target_states();
  myPanel.write_target_states();
}
