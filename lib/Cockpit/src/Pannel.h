#include "Input.h"
#include <Joystick.h>
#include <vector>
#include <memory>
#include <initializer_list>
#include <algorithm>

struct ADSSettings {
    bool enabled;
    uint8_t gain_ADS;
    ADS1115* ADS = nullptr;

    ADSSettings()
        : enabled(false), gain_ADS(0), ADS(nullptr) {}

    ADSSettings(uint8_t gain, ADS1115& ads)
        : enabled(true), gain_ADS(gain), ADS(&ads) {}
};


class Panel{
    private:
        int nb_of_inputs;
        std::vector<std::unique_ptr<Input>> inputs;
        ADSSettings ADS_settings;

    public:
        Panel(std::initializer_list<Input*> lst_inputs, ADSSettings input_ADS_settings)
         : nb_of_inputs(lst_inputs.size()), ADS_settings{input_ADS_settings}
        {
            for (auto ptr : lst_inputs) {
                inputs.emplace_back(ptr);
            }
        }

        void begin(){

            if (ADS_settings.enabled){
                Wire.begin();
                ADS_settings.ADS->begin();
                ADS_settings.ADS->setGain(ADS_settings.gain_ADS);
            }

            for (int i=0; i<nb_of_inputs; i++){
                inputs[i]->begin();
            }
        }

        void update_states(){
            for (int i=0; i<nb_of_inputs; i++){
                inputs[i]->update_state();
            }
        }

        void show_states(){
            for (int i=0; i<nb_of_inputs; i++){
                Serial.print(inputs[i]->id); Serial.print(": "); Serial.print(inputs[i]->state); Serial.print("\t");
            }
            Serial.println();
        }

        Input* operator[](String input_id) const{
            for (int i=0; i<nb_of_inputs; i++){
                if (inputs[i]->id == input_id){
                    return inputs[i].get();
                }
            }
            return nullptr;
        }
};
