#ifndef POT_HPP
#define POT_HPP

#include <Arduino.h>
#include "constants.h"
#include "pins.h"

class Pot
{

    /**
     * @brief 
     * 
     * @return uint8_t 
     */
    static uint8_t readPinPot()
    {
        return analogRead(POTENCIOMETER_PIN);
    }


    /**
     * @brief Get the Voltage object
     * 
     * @return uint8_t 
     */
    static uint8_t getVoltagePot()
    {
        return (SOURCE_VOLTAGE/MAX_VALUE_ADC)*readPinPot();
    }
};

#endif

