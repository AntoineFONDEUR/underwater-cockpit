// #include <Arduino.h>
// #include <Joystick.h>
// #include <Pannel.h>

// Panel myPanel {
//   {
//     // new EncoderInput("encoder1",Pin{26}, 0, 1),
//     new EncoderInput("encoder2",Pin{13}, 2, 3),
//     new EncoderInput("encoder3",Pin{11}, 4, 5),
//     // new EncoderInput("encoder4",Pin{9}, 6, 7),
//     // new EncoderInput("encoder5",Pin{7}, 8, 9),
//     // new EncoderInput("encoder6",Pin{4}, 10, 11),
//     // new EncoderInput("encoder7",Pin{1}, 12, 13),
//     new ButtonInput("switch_toggle1", Pin{29, INPUT_PULLDOWN}, 14),
//     new ButtonInput("switch_toggle2", Pin{28, INPUT_PULLDOWN}, 15),
//     new ButtonInput("switch1", Pin{27, INPUT_PULLDOWN}, 16),
//     new ButtonInput("switch_encoder1", Pin{0, INPUT_PULLUP}, 17),
//     new ButtonInput("switch_encoder2", Pin{3, INPUT_PULLUP}, 18),
//     new ButtonInput("switch_encoder3", Pin{6, INPUT_PULLUP}, 19),
//   },
//   {
//   }
// };

// void setup() {
//   Serial.begin(115200);
//   myPanel.begin();
// }

// void loop() {
//   // Inputs
//   myPanel.read_states();
//   myPanel.send_states();

//   // Ouputs
//   myPanel.receive_target_states();
//   myPanel.write_target_states();

// }

/*
 * Copyright (C) 2023 Giovanni di Dio Bruno
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "pio_encoder.h"

// these are encoders attached to Hare motor control board
PioEncoder encA(11);
PioEncoder encB(13);
PioEncoder encC(26);
PioEncoder encD(9);
// PioEncoder encE(7);
PioEncoder encF(4);
// PioEncoder encG(1);


void setup() {
  encA.begin();
  encB.begin();
  encC.begin();
  encD.begin();
  // encE.begin();
  encF.begin();
  // encG.begin();
  Serial.begin(115200);
  while(!Serial);
}

void loop() {
  Serial.print(encA.getCount());
  Serial.print("\t\t");

  Serial.print(encB.getCount());
  Serial.print("\t\t");

  Serial.print(encC.getCount());
  Serial.print("\t\t");

  Serial.print(encD.getCount());
  Serial.print("\t\t");

  // Serial.print(encE.getCount());
  // Serial.print("\t\t");

  Serial.print(encF.getCount());
  Serial.print("\t\t");

  // Serial.print(encG.getCount());
  Serial.print("\n");

  delay(10);
}
