#include <Arduino.h>
#include <structs.h>
#include <pio_encoder.h>

class Input{
    public:
        String id;
        Pin pin;
        float state;

        Input(String input_id, Pin input_pin)
         : id{input_id}, pin{input_pin} {}

        virtual void begin() = 0;
        virtual void read_state() = 0;
        virtual void send_state() = 0;
};

class AxisInput : public Input {
    private:
        uint16_t get_raw_data(){
            return pin.has_ADS ? pin.ADS->readADC(pin.pin_nb) : analogRead(pin.pin_nb);
        }

    public:
        bool invert;
        uint16_t min_range;
        uint16_t max_range;
        uint8_t num_axis;

        AxisInput(
            String input_id,
            Pin input_pin,
            uint8_t input_num_axis,
            uint16_t input_min_range = 0,
            uint16_t input_max_range = 1024,
            bool input_invert = false)
         : Input(input_id, input_pin), min_range{input_min_range},max_range{input_max_range},invert{input_invert}{
            num_axis = input_num_axis > 5 ? 5 : input_num_axis; //Only six available axis using this Joystick lib
         }

        void begin() override {
            if (!pin.has_ADS){
                pinMode(pin.pin_nb, INPUT);
            }
        }

        void read_state() override{
            float raw_data = get_raw_data();
            float normalized_data = (raw_data-min_range)/(max_range-min_range);
            state = invert ? 1.-normalized_data : normalized_data;
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
        }
};


class ButtonInput : public Input {
    public:
        uint8_t num_button;

        ButtonInput(String input_id, Pin input_pin, uint8_t input_num_button)
         : Input(input_id, input_pin) {
            num_button = input_num_button > 31 ? 31 : input_num_button;
            num_button ++; //Indexing starts at 0 on Cockpit but it starts at 1 for the Joystick lib
        }

        void begin() override{
            pinMode(pin.pin_nb, pin.pin_mode);
        }

        void read_state() override{
            state = digitalRead(pin.pin_nb);
        }

        void send_state() override{
            Joystick.button(num_button, state);
        }

};


class EncoderInput : public Input{
    private:
        int prev_encoder_counter = 0;
        bool is_stable_state(int counter){return counter%4==0;}

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

        void read_state() override{
            int encoder_counter = encoder.getCount();
            if (encoder_counter != prev_encoder_counter){
                if (is_stable_state(prev_encoder_counter)){
                    if (encoder_counter > prev_encoder_counter){
                        state = 1;
                    }
                    else{
                        state = -1;
                    }
                }
                else if (is_stable_state(encoder_counter)){
                    state = 0;
                }
            }
            prev_encoder_counter = encoder_counter;
        }

        void send_state() override{
            if (state < 0){
                Joystick.button(decr_button, true);
            }
            else if (state > 0){
                Joystick.button(incr_button,true);
            }
            else{
                Joystick.button(decr_button, false);
                Joystick.button(incr_button, false);
            }
        }
};
