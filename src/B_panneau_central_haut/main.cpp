#include <Arduino.h>
#include <ADS1X15.h>
#include <Joystick.h>
#include <Pannel.h>

ADS1115 ADS1(0x48);
ADS1115 ADS2(0x49);

Panel myPanel {
  {
    new AxisInput("slider1", Pin{0, ADS1}, 0, 0, 32767),
    new AxisInput("slider2", Pin{1, ADS1}, 1, 0, 32767),
    new AxisInput("slider3", Pin{2, ADS1}, 2, 0, 32767),
    new AxisInput("slider4", Pin{3, ADS1}, 3, 0, 32767),
    new AxisInput("slider5", Pin{0, ADS2}, 4, 0, 32767),
  },
  {
    new PowerPin("high_pin", Pin{15}, HIGH, 1),
    new Led("led1", Pin{29}, 0),
    new Led("led2", Pin{28}, 0),
    new Led("led3", Pin{27}, 0)
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

  // // Ouputs
  myPanel.receive_target_states();
  myPanel.write_target_states();

}
