#include <Input.h>
#include <Output.h>
#include <structs.h>
#include <vector>
#include <memory>
#include <initializer_list>
#include <algorithm>

// Delimiters
#define END_OF_LINE '\n'
#define WORD_DELIMITER ' '

class Panel{
    private:
        int nb_of_inputs;
        int nb_of_outputs;
        std::vector<std::unique_ptr<Input>> inputs;
        std::vector<std::unique_ptr<Output>> outputs;

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
            std::initializer_list<Output*> lst_outputs)
         : nb_of_inputs(lst_inputs.size()), nb_of_outputs(lst_outputs.size())
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
                Joystick.useManualSend(true);
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
                Serial.print(inputs[i]->id); Serial.print(": "); Serial.print(inputs[i]->raw_state); Serial.print("\t");
            }
            Serial.println();
        }

        //Operations on outputs
        void receive_target_states() {
            if (Serial.available() >= 4) {
                uint32_t message = 0;
                for (int i = 0; i < 4; i++) {
                    message |= ((uint32_t)Serial.read() << (8 * i));
                }

                for (int i = 0; i < nb_of_outputs; i++) {
                    int bit_position = outputs[i]->offset;
                    outputs[i]->target_state = (message >> bit_position) & 0x01;
                }
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
