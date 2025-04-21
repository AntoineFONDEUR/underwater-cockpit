#include <Arduino.h>

#ifndef ADS
#include <ADS1X15.h>
#endif

#ifndef STRUCTS
#define STRUCTS

struct Pin {
    uint8_t pin_nb;
    bool has_ADS;
    ADS1115* ADS = nullptr;
    uint8_t pin_mode;

    Pin(uint8_t pin_nb)
        : pin_nb(pin_nb), has_ADS(false), ADS(nullptr) {}

    Pin(uint8_t pin_nb, uint8_t input_pin_mode)
        : pin_nb(pin_nb), has_ADS(false), ADS(nullptr), pin_mode(input_pin_mode) {}

    Pin(uint8_t pin_nb, ADS1115& ads)
        : pin_nb(pin_nb), has_ADS(true), ADS(&ads) {}
};

struct ADSSettings {
    bool enabled;
    uint8_t gain_ADS;
    ADS1115* ADS = nullptr;

    ADSSettings()
        : enabled(false), gain_ADS(0), ADS(nullptr) {}

    ADSSettings(uint8_t gain, ADS1115& ads)
        : enabled(true), gain_ADS(gain), ADS(&ads) {}
};

#endif
