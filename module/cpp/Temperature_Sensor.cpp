//Temperature_Sensor.c
// 
//Description of the stub class for using as temperature sensor.

#include <iostream>

#include "Temperature_Sensor.h"

Temperature_Sensor::Temperature_Sensor(void){
    current_temperature = 0; //TODO: do normal init
    std::cout << "DEBUG: Temp sensor: temp sensor created" << std::endl;
}

double Temperature_Sensor::get_current_temperature(){
    return current_temperature;
}
