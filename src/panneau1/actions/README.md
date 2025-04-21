# Cockpit Configuration

### TODO on Cockpit

- **Data-lake variable**
  - `led-intensity`
    - Type: `number`
    - Initial value: `0`

- **Actions**
  - `switch-led`: `switch-led.js`
  - `disconnect-port`: `disconnect`port.js`
  - `send_params` :
    - `send-params.js`
    - triggered on change of `led-intensity`

- **Joystick Mapping**
  - Button 0: `led-intensity`
  - You can verify that the other inputs work fine on
    - Buttons 1, 2, 3, 4, 5
    - Axis 0, 1, 2, 3, 4

### Other Information

Make sure to activate the `disconnect-port` action once you are done testing.
