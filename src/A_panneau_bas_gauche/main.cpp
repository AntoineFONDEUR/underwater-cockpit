#include <Arduino.h>
#include <ADS1X15.h>
#include <Joystick.h>
#include <Pannel.h>

ADS1115 ADS1(0x48);
ADS1115 ADS2(0x49);

Panel myPanel {
  {
    new AxisInput("joystick1_h", Pin{0, ADS1}, 0, 0, 32767),
    new AxisInput("joystick1_v", Pin{1, ADS1}, 1, 0, 32767),
    new AxisInput("joystick1_tilt", Pin{2, ADS1}, 2, 0, 32767),
    new AxisInput("joystick2_h", Pin{3, ADS1}, 3, 0, 32767),
    new AxisInput("joystick2_v", Pin{1, ADS2}, 4, 0, 32767),
    new AxisInput("joystick2_tilt", Pin{2, ADS2}, 5, 0, 32767),
    new ButtonInput("switch_mode1", Pin{9, INPUT_PULLDOWN}, 0),
    new ButtonInput("switch_mode2", Pin{10, INPUT_PULLDOWN}, 1),
    new ButtonInput("switch_mode3", Pin{11, INPUT_PULLDOWN}, 2),
    new ButtonInput("switch_mode4", Pin{12, INPUT_PULLDOWN}, 3),
    new ButtonInput("switch_mode5", Pin{13, INPUT_PULLDOWN}, 4),
    new ButtonInput("switch_mode6", Pin{14, INPUT_PULLDOWN}, 5),
    new ButtonInput("switch_mode7", Pin{15, INPUT_PULLDOWN}, 6)
  },
  {
    // new Led("led_mode1", Pin{13}),
    // new Led("led_mode2", Pin{12}),
    // new Led("led_mode3", Pin{11}),
    // new Led("led_mode4", Pin{10}),
    // new Led("led_mode5", Pin{9}),
    // new Led("led_mode6", Pin{8}),
    // new Led("led_mode7", Pin{7}),
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
