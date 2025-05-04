#include <Arduino.h>
#include <ADS1X15.h>
#include <Joystick.h>
#include <Pannel.h>

ADS1115 ADS1(0x48);
ADS1115 ADS2(0x49);

const float DEAD_ZONE = 0.05;

Panel myPanel {
  {
    new AxisInput("slider", Pin{0, ADS1}, 0, DEAD_ZONE, true , 32, 27000),
    new AxisInput("joystick_right_h", Pin{1, ADS1}, 1, DEAD_ZONE, false, 32, 27000),
    new AxisInput("joystick_right_v", Pin{3, ADS1}, 3, DEAD_ZONE, false,32, 27000, true),
    new AxisInput("joystick_right_tilt", Pin{2, ADS1}, 2, DEAD_ZONE, false,32, 25500, true),
    new AxisInput("joystick_left_h", Pin{1, ADS2}, 4, DEAD_ZONE, false,32, 27000),
    new AxisInput("joystick_left_v", Pin{2, ADS2}, 5, DEAD_ZONE, false,32, 27000, true),
    new ButtonInput("switch_mode1", Pin{11, INPUT_PULLDOWN}, 0),
    new ButtonInput("switch_mode2", Pin{10, INPUT_PULLDOWN}, 1),
    new ButtonInput("switch_mode3", Pin{15, INPUT_PULLDOWN}, 2),
    new ButtonInput("switch_mode4", Pin{14, INPUT_PULLDOWN}, 3),
    new ButtonInput("switch_mode5", Pin{12, INPUT_PULLDOWN}, 4),
    new ButtonInput("switch_mode6", Pin{13, INPUT_PULLDOWN}, 5),
    new ButtonInput("switch_mode7", Pin{9, INPUT_PULLDOWN}, 6)
  },
  {
    new Led("led-mode1", Pin{0}, 1),
    new Led("led-mode2", Pin{1}, 6),
    new Led("led-mode3", Pin{2}, 5),
    new Led("led-mode4", Pin{3}, 0),
    new Led("led-mode5", Pin{6}, 4),
    new Led("led-mode6", Pin{7}, 3),
    new Led("led-mode7", Pin{8}, 2)
  }
};

void setup() {
  Serial.begin(115200);
  myPanel.begin();

  Wire.begin();
  ADS1.begin();
  ADS1.setGain(1);
  ADS2.begin();
  ADS2.setGain(1);
}

void loop() {
  // Inputs
  myPanel.read_states();
  myPanel.send_states();


  // Ouputs
  myPanel.receive_target_states();
  myPanel.write_target_states();

}
