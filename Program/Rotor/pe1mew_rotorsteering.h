/*--------------------------------------------------------------------
  This file is part of the PE1MEW Arduino Rotor Controller.

  The PE1MEW Arduino Rotor Controller is free software: 
  you can redistribute it and/or modify it under the terms of a Creative 
  Commons Attribution-NonCommercial 4.0 International License 
  (http://creativecommons.org/licenses/by-nc/4.0/) by 
  PE1MEW (http://pe1mew.nl) E-mail: pe1mew@pe1mew.nl

  The PE1MEW Arduino Rotor Controller is distributed in the hope that 
  it will be useful, but WITHOUT ANY WARRANTY; without even the 
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
  PURPOSE.
  --------------------------------------------------------------------*/

 /// \file pe1mew_rotorsteering.h
 /// \brief Steering class for PE1MEW Arduino Rotor Controller
 /// \date 20-7-2016
 /// \author Remko Welling (PE1MEW)
 /// \version 1.0

#ifndef PE1MEW_ROTORSTEERING_H_H
#define PE1MEW_ROTORSTEERING_H_H

#include <stdint.h>

static uint8_t SW1_PIN = 2;							///< Pin on which switch 1 is connected
static uint8_t SW2_PIN = 3;							///< Pin on which switch 2 is connected

static uint8_t INCREMENT_DEFAULT = 4;				///< Default increment step for direction in degrees.

static uint8_t PRESS_COUNTER_MAX = 100;				///< maximum number of steps for the increment counter

/// \brief variables for dynamic increment settings.
static uint8_t PRESS_COUNTER_TRESHOLD1 = 20;	// n * 1 mS wait before next increment
static uint8_t PRESS_COUNTER_TRESHOLD2 = 9;
static uint8_t PRESS_COUNTER_TRESHOLD3 = 3;
static uint8_t PRESS_COUNTER_TRESHOLD4 = 1;

static uint8_t PRESS_COUNTER_STEP1_TRESHOLD	= 10;
static uint8_t PRESS_COUNTER_STEP2_TRESHOLD	= 10;
static uint8_t PRESS_COUNTER_STEP3_TRESHOLD	= 10;
static uint8_t PRESS_COUNTER_STEP4_TRESHOLD	= 0;

/// \brief defines states when reading buttons.
enum eButtonState { BUTTON_NONE = 0x00,		///< No button is pressed
	                BUTTON_1	= 0x01,		///< Only button 1 is pressed
					BUTTON_2	= 0x02,		///< Only button 2 is pressed
					BUTTON_BOTH = 0x03 };	///< Both buttons are pressed

/// \brief defines speed-steps 
/// input by buttons is processed by increments per step.
enum eSetButtonSpeedState { STEP1 = 0,	///< Speed step 1, slowest setting speed
							STEP2,		///< Speed step 2
							STEP3,		///< Speed step 1
							STEP4 };	///< Speed step 1, fastest setting speed

/// \class Rotor steering class
class PE1MEW_RotorSteering
{
public:
	/// \brief Default constructor
    PE1MEW_RotorSteering();
	
	/// \brief Get next direction in degrees from user settings
	/// \return next direction in degrees.
	uint16_t getNextDirection(void){return _NextDirection;}
		
	/// \brief get button state
	/// This function is not private because it is used as input to the test functions.
	/// eButton state is holding all button configurations.
	/// \return button state as eButtonState at the moment of scanning.
	uint8_t getButtons(void);
	
	/// \brief helper function to initialize variables en calculate values for these variables in the constructor of the classes.
	/// Overloaded function form the private function Initialize()
	/// This function is used to initialize the steering control with settings from memory
	/// \value direction in degrees.
	void initialize(uint16_t direction);
			
	/// \brief at sys tick executed function for housekeeping of the Rotor control.
	void Process(void);

private:
	uint8_t  _ProcessVariableIncrement;		///< Step size at increment of a direction
	uint16_t _NextDirection;				///< Next direction in degrees
	uint8_t	 _CurrentButtonSpeedState;		///< Variable for variable speed setting
	uint8_t  _NextButtonSpeedState;			///< Variable for variable speed setting
	uint8_t  _SpeedStateCounter;			///< Variable for variable speed setting
	uint8_t  _ButtonSpeedHoldCounter;		///< Variable for variable speed setting
	bool	 _PressMemory;					///< Variable for variable speed setting
	uint8_t  _PressCounter;					///< Variable for variable speed setting
	
	/// \brief helper function to initialize variables en calculate values for these variables in the constructor of the classes.
	/// Functions are called that cannot be handled by the C++ default initializers
	void	initialize(void);
	
	/// \brief function to scan buttons and in- or decrement next direction.
	/// Function executed at sys tick from process() func
	void	buttonTest(void);
	
	/// \brief 
	void	ProcessButtons(uint8_t inputVariable);
	
	/// \brief Verify angle to fit between 0 and 360 degrees.
	/// \value direction in degrees
	/// \return checked direction in degrees.
	int16_t checkDegreeResult(int16_t degrees);

};

#endif // PE1MEW_ROTORSTEERING_H_H
