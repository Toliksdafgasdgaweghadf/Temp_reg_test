//Temperature_Element.h
// 
//  Description of the stub class used as temperature regulating element.
//  Type of the element is set in the constructor,
// object could be turned on and off (for testing purposes).
// It can also report whether is it a heater or a cooler,
// or return is's current state (true for on, false for off).

#pragma once

enum class Temp_Element_Type{     //Expected temperature element type: heater or cooler
    TEMP_ELEMENT_HEATER,
    TEMP_ELEMENT_COOLER
};

class Temperature_Element{
    public:
        Temperature_Element(Temp_Element_Type type);

        bool is_on(void);    //is this element currently on?
        void turn_on(void);  //turn this element on.
        void turn_off(void); //turn this element off.
        bool is_heater(void);//is this a heater or no?
    private:

        Temp_Element_Type element_type;
        bool is_element_on;
};
