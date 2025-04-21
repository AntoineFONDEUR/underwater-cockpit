#include <Arduino.h>
#include <structs.h>

class Output{
    public:
        String id;
        Pin pin;
        uint16_t target_state = 0;

        Output(String output_id, Pin output_pin)
         : id{output_id}, pin{output_pin} {}

        virtual void begin(){pinMode(pin.pin_nb, OUTPUT);}
        virtual void write_target_state() = 0;
};

class Led : public Output{
    public:
        Led(String output_id, Pin output_pin)
         : Output(output_id, output_pin) {}

        void write_target_state() override{
            digitalWrite(pin.pin_nb, (bool)target_state);
        }
};

class PowerPin : public Output{
    public:
        PowerPin(String output_id, Pin output_pin, bool output_state)
         : Output(output_id, output_pin) {
            target_state = (uint16_t)output_state;
         }

        void write_target_state() override{
            digitalWrite(pin.pin_nb, (bool)target_state);
        }
};
