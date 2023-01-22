# Temp_reg_test
Tets project repository

This is a test project that goes by this requirements:
	Ensure that the temperature in a room is always between a min and max adjustable temperature.
      When temp is too low, start heating, when temp is too high start cooling.

      1- create a component in cpp + api's 
	2- create needed test-cases using google test (stub, mock,..)
	3- code must be build with cmake
	4- code is available in a git repo

The project was made with MS VS2022 in CMake, tests were made using GoogleTest.
Module that controls temperature is described in the Thermostat class,
all test are done in main.cpp

Directory structure:
-root						(root directory, contains main.cpp)
	-module				(containd Thermostat.cpp for the temperature control module and some additional stubs)
		-cpp
		-header
	-test					
		-MOC				(contains specific mock for testing purposes)
			-cpp
			-header 

Module's 'API is simple :
   -set_dependencies() for setting Heater, Cooler and Temperature Sensor references;
   -set_temperature() for setting temperature boundaries;
   -regulation_commit() for trying to regulate temperature.
      Will return True if the temperature is considered in bounds,
      otherwise will return false.
   -is_it_on() for checking working condition.
      Will return true if the regulation is On, otherwise will return false.

adjust_by_rule() method is for testing purposes only, it contains the rule to tune temperature. 
For any real applications it needs to be changed to any desired method of regulation, such as PID or PWM.
