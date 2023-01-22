//Thermostat.h
// 
//  Contains description of the Thermostat class
// with all of the corresponding functions.

#pragma once

#include <iostream>
#include <memory>

#include "Temperature_Element.h"
#include "Temperature_Sensor.h"

enum class Thermostat_Current_Mode {		//Thermostat state enumerator
	THERMOSTAT_STARTING = 0,				//Object was just created, dependencies configuration needed
	THERMOSTAT_READY,						//Object's dependencies were configuret correctly, temperature configuration needed
	THERMOSTAT_HEATING,						//Object is currently trying to increase temperature
	THERMOSTAT_COOLING,						//Object is currently trying to decrease temperature
	THERMOSTAT_IDLE							//Object is currently in idle condition
};

class Thermostat {
public:
	Thermostat(void);

	bool set_dependencies(														//Method for setting corresponding objects references
		                  std::shared_ptr<Temperature_Element> heater,			//A reference to the Heater object
						  std::shared_ptr<Temperature_Element> cooler,			//A reference to the Cooler object
						  std::shared_ptr<Temperature_Sensor> sensor);			//A reference to the Temperature Sensor object

	void set_temperature(double min, double max);								//Method for setting minimum and maximum temperature bounds

	bool regulation_commit(void);												//Method that commits the regulation process

	bool is_it_on(void);														//Method that returns True when the Thermostat is considered on, otherwise it returns false

private:
	void adjust_by_rule(double current_temp);									//Method that contains rules of temperature adjustment

	void regulation_stop(void);													//Method that stops regulation and turns off both Heater and Cooler 
	void regulation_cool(void);													//Method that turns Cooler on
	void regulation_heat(void);													//Method that turns Heater on	


	std::shared_ptr<Temperature_Element> current_heater;						//Reference to Heater object
	std::shared_ptr<Temperature_Element> current_cooler;						//Reference to Cooler object
	std::shared_ptr<Temperature_Sensor>  current_temp_sensor;					//Reference to Temperature Sensor object

	bool is_on;																	//Variable that contains current condition of the Thermostat
	double temp_min;															//Variable that contains minimum allowed temperature value
	double temp_max;															//Variable that contains maximum allowed temperature value
	Thermostat_Current_Mode current_mode;										//Enumerator variable, contains current Thermostat state
};