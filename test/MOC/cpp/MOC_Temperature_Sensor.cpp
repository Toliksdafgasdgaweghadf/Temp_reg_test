//MOC_Temperature_Sensor.cpp
// 
//  Mock module for testing Thermostat class object under specific conditions,
// most of it's functionality is described in the tests.

#include <iostream>

#include "MOC_Temperature_Sensor.h"

MOC_Temperature_Sensor::MOC_Temperature_Sensor(void) {
    fake_heater = nullptr;
    fake_cooler = nullptr;
    temp_step = 0;
    period = 0;
    step_count = 0;
    reg_coeff = default_reg_coeff;
    period_up = true;
    use_temp_regulation = false;
}

void MOC_Temperature_Sensor::moc_set_temperature(double new_temp) {
    current_temperature = new_temp;
}

void MOC_Temperature_Sensor::moc_go_up(double new_temp, double step) {
    moc_set_temperature(new_temp);
    temp_step = step;
    period_up = true;
}

void MOC_Temperature_Sensor::moc_go_down(double new_temp, double step) {
    moc_set_temperature(new_temp);
    temp_step = step;
    period_up = false;
}

void MOC_Temperature_Sensor::moc_go_period(double new_temp, double step, unsigned int period) {
    moc_set_temperature(new_temp);
    temp_step = step;
    period_up = true;
    this->period = period;
}

void MOC_Temperature_Sensor::moc_set_regulatos(std::shared_ptr<Temperature_Element> heater, std::shared_ptr<Temperature_Element> cooler) {
    fake_heater = heater;
    fake_cooler = cooler;
    if(moc_check_regulators())
    {
        moc_use_temp_regulation(true);
        std::cout << "INFO: MOCK Temp sensor: heater and cooler detected, starting temperature regulation" << std::endl;
    }
}

void MOC_Temperature_Sensor::moc_use_temp_regulation(bool param_state)
{
    use_temp_regulation = param_state;
}

bool MOC_Temperature_Sensor::moc_check_regulators(void)
{
    return (fake_heater != nullptr && fake_cooler != nullptr);
}

void  MOC_Temperature_Sensor::moc_regulation_coefficient(double new_coeff)
{
    reg_coeff = new_coeff;
}

void MOC_Temperature_Sensor::moc_process(void)
{
    if (period > 0)
    {
        if (step_count < period)    //period is not over - temperature is changing
        {
            ++step_count;
        }
        else                        //period is over - we need to change temperature direction
        {
            step_count = 0;
            period_up = !period_up;
        }
    }

    if (period_up)
    {
        current_temperature += temp_step;
    }
    else
    {
        current_temperature -= temp_step;
    }

    if (use_temp_regulation && moc_check_regulators())  //if we're trying to adjust temperature accordingly to the temperature regulators' state
    {
        if (fake_heater->is_on())
        {
            current_temperature += temp_step * reg_coeff;
        }
        if (fake_cooler->is_on())
        {
            current_temperature -= temp_step * reg_coeff;
        }
    }
    std::cout << "INFO: MOCK Temp sensor: current temperature is " << current_temperature << " degrees" << std::endl;
}
