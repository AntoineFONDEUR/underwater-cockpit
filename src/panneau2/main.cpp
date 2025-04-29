#include <Arduino.h>
#include <ADS1X15.h>
#include <Joystick.h>
#include <Pannel.h>

ADS1115 ADS(0x48);

Panel myPanel {
  {
    // new ButtonInput("switch1", Pin{6, INPUT_PULLDOWN}, 0),
    // new ButtonInput("switch2", Pin{7, INPUT_PULLDOWN}, 1),
    // new ButtonInput("switch3", Pin{8, INPUT_PULLDOWN}, 2),
    // new ButtonInput("switch4", Pin{9, INPUT_PULLDOWN}, 3),
    // new AxisInput("joystick_v", Pin{0, ADS}, 0, 0, 32767),
    // new AxisInput("joystick_h", Pin{1, ADS}, 1, 0, 32767),
    // new AxisInput("joystick_tilt", Pin{2, ADS}, 2, 0, 32767),
    // new AxisInput("slider", Pin{3, ADS}, 3, 0, 32767)
    new EncoderInput("encoder1", Pin{1}, 1, 2)
  },
  {

  },
  ADSSettings{1,ADS}
};

void setup() {
  Serial.begin(9600);
  myPanel.begin();
}

void loop() {
  // Inputs
  myPanel.read_states();
  myPanel.send_states();

  // Ouputs
  myPanel.receive_target_states();
  myPanel.write_target_states();

}
