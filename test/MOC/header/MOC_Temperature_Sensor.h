//MOC_Temperature_Sensor.h
// 
//  Mock module for testing Thermostat class object under specific conditions,
// most of it's functionality is described in the tests.

#pragma once

#include <memory>

#include "Temperature_Sensor.h"
#include "Temperature_Element.h"

class MOC_Temperature_Sensor : public Temperature_Sensor {
public:
    MOC_Temperature_Sensor(void);

    void moc_set_temperature(double new_temp);

    void moc_go_up(double new_temp, double step);
    void moc_go_down(double new_temp, double step);
    void moc_go_period(double new_temp, double step, unsigned int period);

    void moc_set_regulatos(std::shared_ptr<Temperature_Element> heater, std::shared_ptr<Temperature_Element> cooler);
    void moc_use_temp_regulation(bool param_state);
    void moc_regulation_coefficient(double new_coeff);
    void moc_process(void);

    const double default_reg_coeff = 1.25;       //default value of the temperature regulation coefficient for the temperature agents

private:
    bool moc_check_regulators(void);

    std::shared_ptr<Temperature_Element> fake_heater;
    std::shared_ptr<Temperature_Element> fake_cooler;
    double temp_step;
    int period;
    bool period_up;
    int step_count;
    bool use_temp_regulation;
    double reg_coeff;
};