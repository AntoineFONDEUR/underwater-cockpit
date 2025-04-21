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
    new AxisInput("left_joystick_h", Pin{0, ADS}, 3, 0, 32767),
    new AxisInput("left_joystick_v", Pin{1, ADS}, 4, 0, 32767),
    new ButtonInput("silver_switch", Pin{0, INPUT_PULLDOWN}, 0),
    new ButtonInput("encoder_switch", Pin{15, INPUT_PULLDOWN}, 1),
    new ButtonInput("left_joystick_switch", Pin{3, INPUT_PULLDOWN}, 2),
    new ButtonInput("right_joystick_switch", Pin{28, INPUT_PULLDOWN}, 3),
    new EncoderInput("encoder", Pin{1}, 4, 5)
  },
  {
    new Led("led-intensity", Pin{26}),
    new PowerPin("low_pin1", Pin{6}, LOW)
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
