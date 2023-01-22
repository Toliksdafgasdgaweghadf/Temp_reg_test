//Thermostat.cpp
// 
//  Contains description of the Thermostat class
// with all of the corresponding functions.
//  Thermostat object is responsible for the temperature regulation:
// it tunes Heater and Cooler objects according to the temperature obtained from 
// the Temperature Sensor object, using adjust_by_rule() method for temperature correction.
// adjust_by_rule() method is for testing purposes only, it needs to be changed to 
// any desired method of regulation, such as PID or PWM.
//
//Module's 'API is simple :
//   -set_dependencies() for setting Heater, Cooler and Temperature Sensor references;
//   -set_temperature() for setting temperature boundaries;
//   -regulation_commit() for trying to regulate temperature.
//      Will return True if the temperature is considered in bounds,
//      otherwise will return false.
//   -is_it_on() for checking working condition.
//      Will return true if the regulation is On, otherwise will return false.


#include <iostream>
#include <memory>
#include <cstdlib>

#include "Thermostat.h"
#include "Temperature_Element.h"
#include "Temperature_Sensor.h"

Thermostat::Thermostat(void)
{
	temp_min = 0;
	temp_max = 0;
	current_mode = Thermostat_Current_Mode::THERMOSTAT_STARTING;
	
	is_on = false;
}

bool Thermostat::is_it_on(void)
{
	return is_on;
}

bool Thermostat::set_dependencies(std::shared_ptr<Temperature_Element> heater, std::shared_ptr<Temperature_Element> cooler, std::shared_ptr<Temperature_Sensor> sensor)
{
	if (heater == nullptr)
	{
		return false;
	}
	if (!heater->is_heater())
	{
		return false;
	}
	if (cooler == nullptr)
	{
		return false;
	}
	if (cooler->is_heater())
	{
		return false;
	}
	if (sensor == nullptr)
	{
		return false;
	}

	current_heater = heater;
	current_cooler = cooler;
	current_temp_sensor = sensor;
	current_mode = Thermostat_Current_Mode::THERMOSTAT_READY;	//Thermostat is ready for setting temperature

	return true;
}

void Thermostat::regulation_stop(void)
{
	if (current_heater->is_on())
	{
		current_heater->turn_off();
	}
	if (current_cooler->is_on())
	{
		current_cooler->turn_off();
	}

	current_mode = Thermostat_Current_Mode::THERMOSTAT_IDLE;
}

void Thermostat::regulation_cool(void)
{
	current_cooler->turn_on();
	current_mode = Thermostat_Current_Mode::THERMOSTAT_COOLING;
}

void Thermostat::regulation_heat(void)
{
	current_heater->turn_on();
	current_mode = Thermostat_Current_Mode::THERMOSTAT_HEATING;
}


void Thermostat::set_temperature(double min, double max)
{
	if (current_mode == Thermostat_Current_Mode::THERMOSTAT_STARTING)
	{
		std::cout << "ERROR: Thermostat setup error: dependencies setup needed" << std::endl;
		return;
	}
	if (min > max) 
	{
		std::cout << "ERROR: Thermostat setup error: min temp is bigger than max temp" << std::endl;
		return;
	}
	if(min == max) 
	{
		std::cout << "ERROR: Thermostat setup error: min temp is equal to max temp" << std::endl;
		return;
	}

	temp_max = max;
	temp_min = min;
	is_on = true;
	std::cout << "ERROR: Thermostat: temperature setup completed" << std::endl;
}

bool Thermostat::regulation_commit(void)
{
	if (!is_on)
	{
		std::cout << "ERROR: Thermostat is off, can't do adjustment" << std::endl;
		return false;
	}

	double current_temperature = current_temp_sensor->get_current_temperature();
	adjust_by_rule(current_temperature);
	return (current_mode == Thermostat_Current_Mode::THERMOSTAT_IDLE);		//if temperature is in bounds - return true, otherwise - return false
}

void Thermostat::adjust_by_rule(double current_temp)		//basic rule to adjust temberature by
{
	double average_temp = (temp_max + temp_min)/2;
	double temp_difference = current_temp - average_temp;

	const double regulation_bound = 13;

	if( abs(temp_difference) < regulation_bound)
	{
		if (current_mode != Thermostat_Current_Mode::THERMOSTAT_IDLE) 
		{
			std::cout << "INFO: Thermostat: temperature is in bounds, going to idle state" << std::endl;
			regulation_stop();
		}
		return;
	}
	else {
		if (temp_difference < 0 && current_mode != Thermostat_Current_Mode::THERMOSTAT_HEATING)
		{
			std::cout << "INFO: Thermostat: temperature is lower than minimum, starting heater" << std::endl;
			regulation_heat();
		}
		if (temp_difference > 0 && current_mode != Thermostat_Current_Mode::THERMOSTAT_COOLING)
		{
			std::cout << "INFO: Thermostat: temperature is higher than maximum, starting cooler" << std::endl;
			regulation_cool();
		}
	}
}
