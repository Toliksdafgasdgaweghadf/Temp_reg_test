//Temperature_Sensor.h
// 
//Description of the stub class for using as temperature sensor.

#pragma once

class Temperature_Sensor{
    public:
        Temperature_Sensor(void);
        
        double get_current_temperature(void);           //returns current temperature detected by the sensor.
    protected:
        double current_temperature;                     //stores current temperature detected by the sensor.

};
