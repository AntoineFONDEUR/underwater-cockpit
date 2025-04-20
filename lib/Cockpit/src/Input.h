#include <Arduino.h>

struct Pin {
    uint8_t pin_nb;
    bool has_ADS;
    ADS1115* ADS = nullptr;

    Pin(uint8_t pin_nb)
        : pin_nb(pin_nb), has_ADS(false), ADS(nullptr) {}

    Pin(uint8_t pin_nb, ADS1115& ads)
        : pin_nb(pin_nb), has_ADS(true), ADS(&ads) {}
};

class Input{
    public:
        String id;
        Pin pin;
        float state;

        Input(String input_id, Pin input_pin)
         : id{input_id}, pin{input_pin} {}

        virtual void begin() = 0;
        virtual void update_state() = 0;
};

class AxisInput : public Input {
    private:
        uint16_t get_raw_data(){
            if (!pin.has_ADS){
                return analogRead(pin.pin_nb);
            }
            else{
                return pin.ADS->readADC(pin.pin_nb);
            }
            return 0;
        }
    public:
        bool invert;
        uint16_t min_range;
        uint16_t max_range;

        AxisInput(
            String input_id,
            Pin input_pin,
            uint16_t input_min_range = 0,
            uint16_t input_max_range = 1024,
            bool input_invert = false)
         : Input(input_id, input_pin),min_range{input_min_range},max_range{input_max_range},invert{input_invert}{}

        void begin() override {
            if (!pin.has_ADS){
                pinMode(pin.pin_nb, INPUT);
            }
        }

        void update_state(){
            float raw_data = get_raw_data();
            float normalized_data = (raw_data-min_range)/(max_range-min_range);
            state = invert ? 1.-normalized_data : normalized_data;
        }
};
