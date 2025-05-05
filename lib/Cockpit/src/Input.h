#include <Arduino.h>
#include <structs.h>
#include <pio_encoder.h>

class Input{
    public:
        String id;
        Pin pin;
        float state;
        float raw_state = 0;

        Input(String input_id, Pin input_pin)
         : id{input_id}, pin{input_pin} {}

        virtual void begin() = 0;
        virtual void read_state() = 0;
        virtual void send_state() = 0;
};

class AxisInput : public Input {
    private:
        float dead_zone;
        bool slider;
        float prev_state = -42.;
        uint16_t get_raw_data(){
            return pin.has_ADS ? pin.ADS->readADC(pin.pin_nb) : analogRead(pin.pin_nb);
        }

    public:
        bool invert;
        uint16_t min_range;
        uint16_t max_range;
        uint8_t num_axis;
        float maximum_variation;

        AxisInput(
            String input_id,
            Pin input_pin,
            uint8_t input_num_axis,
            float input_dead_zone,
            bool input_slider = false,
            uint16_t input_min_range = 0,
            uint16_t input_max_range = 1024,
            bool input_invert = false,
            float input_maximum_variation = 1.)
         : Input(input_id, input_pin), min_range{input_min_range},max_range{input_max_range},invert{input_invert},
           dead_zone{input_dead_zone}, slider{input_slider}, maximum_variation(input_maximum_variation){
            num_axis = input_num_axis > 5 ? 5 : input_num_axis; //Only six available axis using this Joystick lib
         }

        void begin() override {
            if (!pin.has_ADS){
                pinMode(pin.pin_nb, INPUT);
            }
        }

        void read_state() override{
            float raw_data = get_raw_data();
            raw_state = raw_data;
            // normalize and reverse as needed :
            float normalized_data = (raw_data-min_range)/(max_range-min_range);
            state = invert ? 1.-normalized_data : normalized_data;
            // dead-zone for joysticks
            if (!slider){
                state = ( - dead_zone < (0.5 - state) & (0.5 - state) < dead_zone ) ? 0.5 : state;
            }
            // cut off fast changes
            if (prev_state<-40.){
                prev_state = state;
            }
            if (id == "slider"){
                Serial.println(state);
                Serial.println(prev_state);
            }
            if (!(- maximum_variation <= state-prev_state & state-prev_state <= maximum_variation)){
                Serial.print(id); Serial.print(" going too fast: "); Serial.println(state-prev_state);
                state = prev_state;
            }
            else{
                prev_state = state;
            }
        }

        void send_state() override{
            Joystick.use10bit();
            if (num_axis == 0){
                Joystick.X(1024*state);
            }
            else if (num_axis == 1){
                Joystick.Y(1024*state);
            }
            else if (num_axis == 2){
                Joystick.Z(1024*state);
            }
            else if (num_axis == 3){
                Joystick.sliderLeft(1024*state);
            }
            else if (num_axis == 4){
                Joystick.sliderRight(1024*state);
            }
            else if (num_axis == 5){
                Joystick.Zrotate(1024*state);
            }
            Joystick.send_now();

        }
};


class ButtonInput : public Input {
    private:
        uint8_t debounce_delay;
        uint8_t reading;
        uint8_t previous_state;
    public:
        uint8_t num_button;

        ButtonInput(String input_id, Pin input_pin, uint8_t input_num_button, uint8_t input_debounce_delay = 20)
         : Input(input_id, input_pin), debounce_delay{input_debounce_delay}{
            num_button = input_num_button > 31 ? 31 : input_num_button;
            num_button ++; //Indexing starts at 0 on Cockpit but it starts at 1 for the Joystick lib
        }

        void begin() override{
            pinMode(pin.pin_nb, pin.pin_mode);
            reading = digitalRead(pin.pin_nb);
            previous_state = reading;
        }

        void read_state() override{
            static uint8_t last_debounce_time = 0;

            int reading = digitalRead(pin.pin_nb);

            if (reading != previous_state) {
                last_debounce_time = millis();
            }

            if ((millis() - last_debounce_time) > debounce_delay) {
                if (reading != state) {
                    state = reading;
                }
            }

            previous_state = reading;

        }

        void send_state() override{
            state = pin.pin_mode == INPUT_PULLUP ? 1 - state : state;
            Joystick.button(num_button, state);
            Joystick.send_now();
        }

};


class EncoderInput : public Input{
    private:
        int prev_stable_counter = 0;
        int counter = 0;

    public:
        uint8_t decr_button;
        uint8_t incr_button;
        PioEncoder encoder;

        EncoderInput(String input_id, Pin input_pin, uint8_t input_decr_button, uint8_t input_incr_button)
         : Input(input_id, input_pin), encoder{input_pin.pin_nb}{
            decr_button = input_decr_button > 31 ? 31 : input_decr_button;
            decr_button ++;
            incr_button = input_incr_button > 31 ? 31 : input_incr_button;
            incr_button ++;
        }

        void begin() override{
            encoder.begin();
        }

        void read_state() override {
            counter = encoder.getCount();
            int diff = counter - prev_stable_counter;
            int current_stable =  (-4 < diff & diff < 4) ? prev_stable_counter : counter  - (counter % 4);
            int steps = (current_stable - prev_stable_counter) / 4;

            state = steps;
            prev_stable_counter = current_stable;
        }

        void send_state() override {
            static int pending_steps = 0;
            static bool pressing = false;
            static uint8_t current_button = 0;
            static unsigned long press_start_time = 0;

            pending_steps += state;
            state = 0;

            if (!pressing && pending_steps != 0) {
                pressing = true;
                press_start_time = millis();

                if (pending_steps > 0) {
                    current_button = incr_button;
                    pending_steps--;
                } else {
                    current_button = decr_button;
                    pending_steps++;
                }

                Joystick.button(current_button, true);
                Joystick.send_now();

            }

            if (pressing && (millis() - press_start_time >= 30)) {
                Joystick.button(current_button, false);
                Joystick.send_now();

                pressing = false;
            }
        }
};
