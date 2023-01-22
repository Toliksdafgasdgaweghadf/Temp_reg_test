//Main.cpp
//Main project file, contains all the tests involved.

#include <iostream>
#include <memory>

#include <gtest/gtest.h>

#include "Thermostat.h"
#include "Temperature_Element.h"
#include "Temperature_Sensor.h"

#include "MOC_Temperature_Sensor.h"

const double temperature_max = 15;                  //Upper temperature bound
const double temperature_min = -15;                 //Lower temperature bound        

TEST(Termostat, Object_Creation) {                  //Basic object creation test
    Thermostat My_Thermostat;
    
    EXPECT_EQ(My_Thermostat.is_it_on(), false);     //Thermostat is off by default
}

TEST(Termostat, Setting_Dependencies) {             //Dependencies test: will the Thermostat run without crucial components or with those components mismatched?
    Thermostat My_Thermostat;
    My_Thermostat.set_dependencies(nullptr, nullptr, nullptr);

    EXPECT_EQ(My_Thermostat.set_dependencies(nullptr, nullptr, nullptr), false);     //Thermostat error - dependencies init failed

    auto Heater = std::make_shared< Temperature_Element >(Temp_Element_Type::TEMP_ELEMENT_HEATER);
    EXPECT_EQ(My_Thermostat.set_dependencies(Heater, nullptr, nullptr), false);     //Thermostat is off - dependencies init failed again

    auto Cooler = std::make_shared< Temperature_Element >(Temp_Element_Type::TEMP_ELEMENT_COOLER);
    EXPECT_EQ(My_Thermostat.set_dependencies(Heater, Cooler, nullptr), false);     //Thermostat is off - dependencies init failed again

    EXPECT_EQ(My_Thermostat.set_dependencies(Cooler, Cooler, nullptr), false);     //Thermostat is off - dependencies init failed again

    EXPECT_EQ(My_Thermostat.set_dependencies(Heater, Heater, nullptr), false);     //Thermostat is off - dependencies init failed again

    auto Sensor = std::make_shared< Temperature_Sensor >();
    EXPECT_EQ(My_Thermostat.set_dependencies(Heater, Cooler, Sensor), true);     //Thermostat is on - dependencies init successful
}

TEST(Termostat, Setting_Temperature) {              //Testing the setting of temperature bounds
    Thermostat My_Thermostat;
   
    auto Heater = std::make_shared< Temperature_Element >(Temp_Element_Type::TEMP_ELEMENT_HEATER);
    auto Cooler = std::make_shared< Temperature_Element >(Temp_Element_Type::TEMP_ELEMENT_COOLER);
    auto Sensor = std::make_shared< Temperature_Sensor >();
    My_Thermostat.set_dependencies(Heater, Cooler, Sensor);
    EXPECT_EQ(My_Thermostat.is_it_on(), false);     //Thermostat is off - dependencies init successful

    My_Thermostat.set_temperature(temperature_min, temperature_max);
    EXPECT_EQ(My_Thermostat.is_it_on(), true);     //Thermostat is on - temperature setup completed
}

TEST(Termostat, Basic_Regulation) {                 //Basic temperature regulation test
    Thermostat My_Thermostat;

    auto Heater = std::make_shared< Temperature_Element >(Temp_Element_Type::TEMP_ELEMENT_HEATER);
    auto Cooler = std::make_shared< Temperature_Element >(Temp_Element_Type::TEMP_ELEMENT_COOLER);
    auto MOC_Sensor = std::make_shared< MOC_Temperature_Sensor >();
    My_Thermostat.set_dependencies(Heater, Cooler, MOC_Sensor);
    My_Thermostat.set_temperature(temperature_min, temperature_max);

    MOC_Sensor->moc_go_down(100, 10);               //Mock sensor configured to sweep temperature from 100 to minus infinity with the step of 10                        

    int cycle_count = 10;                           //Cycle counter was set for the temperature to stop in desired bounds
    while (cycle_count > 0) {
        MOC_Sensor->moc_process();
        My_Thermostat.regulation_commit();
        --cycle_count;
    }

    EXPECT_EQ(My_Thermostat.regulation_commit(), true);     //Thermostat is idle - temperature is in bounds
}

TEST(Termostat, Regulation_From_Upper_To_Set) {       //Temperature regulation test: temperature goes from upper value to the desired one
    Thermostat My_Thermostat;

    auto Heater = std::make_shared< Temperature_Element >(Temp_Element_Type::TEMP_ELEMENT_HEATER);
    auto Cooler = std::make_shared< Temperature_Element >(Temp_Element_Type::TEMP_ELEMENT_COOLER);
    auto MOC_Sensor = std::make_shared< MOC_Temperature_Sensor >();
    My_Thermostat.set_dependencies(Heater, Cooler, MOC_Sensor);
    My_Thermostat.set_temperature(temperature_min, temperature_max);

    MOC_Sensor->moc_go_down(100, 2);                 //Mock sensor configured to sweep temperature from 100 to minus infinity with the step of 2
    MOC_Sensor->moc_set_regulatos(Heater, Cooler);   //Mock temperature regulators influence on the temperature is turned on

    int cycle_count = 50;
    while (cycle_count > 0) {
        MOC_Sensor->moc_process();
        My_Thermostat.regulation_commit();
        --cycle_count;
    }

    EXPECT_EQ(My_Thermostat.regulation_commit(), true);     //Thermostat is idle - temperature is in bounds
}

TEST(Termostat, Regulation_From_Lower_To_Set) {     //Temperature regulation test: temperature goes from lower value to the desired one
    Thermostat My_Thermostat;

    auto Heater = std::make_shared< Temperature_Element >(Temp_Element_Type::TEMP_ELEMENT_HEATER);
    auto Cooler = std::make_shared< Temperature_Element >(Temp_Element_Type::TEMP_ELEMENT_COOLER);
    auto MOC_Sensor = std::make_shared< MOC_Temperature_Sensor >();
    My_Thermostat.set_dependencies(Heater, Cooler, MOC_Sensor);
    My_Thermostat.set_temperature(temperature_min, temperature_max);

    MOC_Sensor->moc_go_up(-100, 2);                 //Mock sensor configured to sweep temperature from -100 to plus infinity with the step of 2
    MOC_Sensor->moc_set_regulatos(Heater, Cooler);  //Mock temperature regulators influence on the temperature is turned on

    int cycle_count = 50;
    while (cycle_count > 0) {
        MOC_Sensor->moc_process();
        My_Thermostat.regulation_commit();
        --cycle_count;
    }

    EXPECT_EQ(My_Thermostat.regulation_commit(), true);     //Thermostat is idle - temperature is in bounds
}

TEST(Termostat, Regulation_Period_To_Set) {     //Temperature regulation test: temperature goes periodicaly around some value, setting to the desired one
    Thermostat My_Thermostat;

    auto Heater = std::make_shared< Temperature_Element >(Temp_Element_Type::TEMP_ELEMENT_HEATER);
    auto Cooler = std::make_shared< Temperature_Element >(Temp_Element_Type::TEMP_ELEMENT_COOLER);
    auto MOC_Sensor = std::make_shared< MOC_Temperature_Sensor >();
    My_Thermostat.set_dependencies(Heater, Cooler, MOC_Sensor);
    My_Thermostat.set_temperature(temperature_min, temperature_max);

    MOC_Sensor->moc_go_period(-100, 2, 10);          //Mock sensor configured to sweep temperature from -100 to with the step of 2, changing the direction of the sweep every 10 steps 
    MOC_Sensor->moc_set_regulatos(Heater, Cooler);   //Mock temperature regulators influence on the temperature is turned on

    int cycle_count = 50;
    while (cycle_count > 0) {
        MOC_Sensor->moc_process();
        My_Thermostat.regulation_commit();
        --cycle_count;
    }

    EXPECT_EQ(My_Thermostat.regulation_commit(), true);     //Thermostat is idle - temperature is in bounds
}
