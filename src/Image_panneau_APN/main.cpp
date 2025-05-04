#include <Arduino.h>
#include <Joystick.h>
#include <Pannel.h>

Panel myPanel {
  {
    new EncoderInput("encoder1",Pin{26}, 0, 1),
    new EncoderInput("encoder2",Pin{13}, 2, 3),
    new EncoderInput("encoder3",Pin{11}, 4, 5),
    new EncoderInput("encoder4",Pin{9}, 6, 7),
    new EncoderInput("encoder5",Pin{7}, 8, 9),
    new EncoderInput("encoder6",Pin{4}, 10, 11),
    new EncoderInput("encoder7",Pin{1}, 12, 13),
    new ButtonInput("switch1", Pin{15}, 14),
    new ButtonInput("switch2", Pin{28}, 15),
    new ButtonInput("switch3", Pin{29}, 16),
    new ButtonInput("switch_encoder1", Pin{6}, 17),
    new ButtonInput("switch_encoder2", Pin{3}, 18),
    new ButtonInput("switch_encoder3", Pin{0}, 19),
  },
  {
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
