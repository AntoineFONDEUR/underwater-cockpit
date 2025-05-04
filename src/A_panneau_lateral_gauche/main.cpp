#include <Arduino.h>
#include <Joystick.h>
#include <Pannel.h>

Panel myPanel {
  {
    new AxisInput("gain-pot", Pin{29},0,0,true,0,1024,true),
    new ButtonInput("switch1", Pin{0, INPUT_PULLDOWN}, 0),
    new ButtonInput("switch2", Pin{7, INPUT_PULLDOWN}, 1),
    new ButtonInput("switch3", Pin{15, INPUT_PULLDOWN}, 2),
    new ButtonInput("switch4", Pin{14, INPUT_PULLDOWN}, 3),
    new ButtonInput("switch5", Pin{2, INPUT_PULLDOWN}, 4),
    new ButtonInput("switch6", Pin{3, INPUT_PULLDOWN}, 5),
    new ButtonInput("switch7", Pin{4, INPUT_PULLDOWN}, 6),
  },
  {
    new PowerPin("low_pin", Pin{5}, LOW, 1),
    new PowerPin("high_pin", Pin{6}, HIGH, 1),
    new Led("led-arm", Pin{1},0),
    new Led("led1", Pin{7},1),
    new Led("led2", Pin{8},2),
    new Led("led3", Pin{9},3),
    new Led("led4", Pin{10},4),
    new Led("led5", Pin{11},5),
    new Led("led6", Pin{12},6),
    new Led("led7", Pin{13},7),
  }
};

void setup() {
  Serial.begin(115200);
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
