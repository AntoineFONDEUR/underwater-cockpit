#include <Input.h>
#include <Output.h>
#include <vector>
#include <memory>
#include <initializer_list>
#include <algorithm>

// Delimiters
#define END_OF_LINE '\n'
#define WORD_DELIMITER ' '

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
        int nb_of_outputs;
        std::vector<std::unique_ptr<Input>> inputs;
        std::vector<std::unique_ptr<Output>> outputs;
        ADSSettings ADS_settings;

        //Serial communication methods
        String read_line() {
            static String inputString = "";

            while (Serial.available()) {
                char input_char = (char)Serial.read();

                if (input_char == END_OF_LINE) {
                    String result = inputString;
                    inputString = "";
                    return result;
                } else {
                    inputString += input_char;
                }
            }

            return "";
        }

        void parse_update_msg(String received, String& id, uint16_t& value){
            received.trim();

            int word_delimiter_pos = received.indexOf(WORD_DELIMITER);

            id = received.substring(0, word_delimiter_pos);
            value = received.substring(word_delimiter_pos + 1).toInt();
        }

    public:
        Panel(
            std::initializer_list<Input*> lst_inputs,
            std::initializer_list<Output*> lst_outputs,
            ADSSettings input_ADS_settings)
         : nb_of_inputs(lst_inputs.size()), nb_of_outputs(lst_outputs.size()), ADS_settings{input_ADS_settings}
        {
            for (auto ptr : lst_inputs) {
                inputs.emplace_back(ptr);
            }
            for (auto ptr : lst_outputs) {
                outputs.emplace_back(ptr);
            }
        }

        void begin(){

            if (nb_of_inputs > 0){
                Joystick.begin();
            }

            if (ADS_settings.enabled){
                Wire.begin();
                ADS_settings.ADS->begin();
                ADS_settings.ADS->setGain(ADS_settings.gain_ADS);
            }

            for (int i=0; i<nb_of_inputs; i++){
                inputs[i]->begin();
            }

            for (int i=0; i<nb_of_outputs; i++){
                outputs[i]->begin();
            }
        }

        // Operations on inputs
        void read_states(){
            for (int i=0; i<nb_of_inputs; i++){
                inputs[i]->read_state();
            }
        }

        void send_states(){
            for (int i=0; i<nb_of_inputs; i++){
                inputs[i]->send_state();
            }
        }

        void show_states(){
            for (int i=0; i<nb_of_inputs; i++){
                Serial.print(inputs[i]->id); Serial.print(": "); Serial.print(inputs[i]->state); Serial.print("\t");
            }
            Serial.println();
        }

        //Operations on outputs
        void receive_target_states(){
            String received = read_line();
            if (received.length() > 0) {
                String output_id;
                uint16_t output_target_state;
                parse_update_msg(received, output_id, output_target_state);
                get_output(output_id)->target_state = output_target_state;
            }
        }

        void write_target_states(){
            for (int i=0; i<nb_of_outputs; i++){
                outputs[i]->write_target_state();
            }
        }

        // Getters
        Input* get_input(String input_id) const{
            for (int i=0; i<nb_of_inputs; i++){
                if (inputs[i]->id == input_id){
                    return inputs[i].get();
                }
            }
            return nullptr;
        }

        Output* get_output(String output_id) const{
            for (int i=0; i<nb_of_outputs; i++){
                if (outputs[i]->id == output_id){
                    return outputs[i].get();
                }
            }
            return nullptr;
        }
};
