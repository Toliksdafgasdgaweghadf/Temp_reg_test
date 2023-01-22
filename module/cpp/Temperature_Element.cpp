//Temperature_Element.cpp
// 
//  Description of the stub class used as temperature regulating element.
//  Type of the element (heater or cooler) is set in the constructor,
// object could be turned on and off (for testing purposes).
// It can also report whether is it a heater or a cooler,
// or return is's current state (true for on, false for off).

#include <iostream>
#include <string>

#include "Temperature_Element.h"

Temperature_Element::Temperature_Element(Temp_Element_Type type){
    element_type = type;
    is_element_on = false;
}

bool Temperature_Element::is_on(void){
    return this->is_element_on;
}

void Temperature_Element::turn_on(void){
    this->is_element_on = true;
}

void Temperature_Element::turn_off(void){
    this->is_element_on = false;
}

bool Temperature_Element::is_heater(void){
    return (this->element_type == Temp_Element_Type::TEMP_ELEMENT_HEATER);
}