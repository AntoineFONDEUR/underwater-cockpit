#include <Arduino.h>

class Output{
    public:
        String id;
        uint8_t pin;
        uint16_t target_state = 0;

        Output(String input_id, uint8_t input_pin)
         : id{input_id}, pin{input_pin} {}

        virtual void begin(){pinMode(pin, OUTPUT);}
        virtual void write_target_state() = 0;
};

class Led : public Output{
    public:
        Led(String input_id, uint8_t input_pin)
         : Output(input_id, input_pin) {}

        void write_target_state() override{
            digitalWrite(pin, (bool)target_state);
        }
};
