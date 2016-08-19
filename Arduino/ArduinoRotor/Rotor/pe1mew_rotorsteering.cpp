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

 /// \file pe1mew_rotorsteering.cpp
 /// \brief Steering class for PE1MEW Arduino Rotor Controller
 /// \date 20-7-2016
 /// \author Remko Welling (PE1MEW)
 /// \version 1.0

#include "pe1mew_rotorsteering.h"

#include "Arduino.h"

PE1MEW_RotorSteering::PE1MEW_RotorSteering():
	_ProcessVariableIncrement(INCREMENT_DEFAULT),
	_NextDirection(0),
	_CurrentButtonSpeedState(STEP1),
	_NextButtonSpeedState(STEP1),
	_SpeedStateCounter(0),
	_ButtonSpeedHoldCounter(0),
	_PressMemory(false),
	_PressCounter(PRESS_COUNTER_MAX)
{
	initialize();
}

void PE1MEW_RotorSteering::initialize(void)
{
	pinMode(SW1_PIN, INPUT);
	pinMode(SW2_PIN, INPUT);
}

void PE1MEW_RotorSteering::initialize(uint16_t direction)
{
	pinMode(SW1_PIN, INPUT);
	pinMode(SW2_PIN, INPUT);
	_NextDirection = direction;
}

void PE1MEW_RotorSteering::Process(void)
{
	// test for button press
	buttonTest();
}

void PE1MEW_RotorSteering::buttonTest(void)
{
	// read button status
	uint8_t inputVariable = getButtons();
	
	if (inputVariable != 0x00)
	{
		_PressMemory = true;
	}
	else
	{
		_PressMemory = false;
	}
	

	if (_PressMemory)
	{
		if (_ButtonSpeedHoldCounter < PRESS_COUNTER_MAX)
		{
			_ButtonSpeedHoldCounter++;		
		}
		
		switch (_CurrentButtonSpeedState)
		{
			case STEP1:
				if (PRESS_COUNTER_TRESHOLD1 < _ButtonSpeedHoldCounter)
				{
					if (_SpeedStateCounter < PRESS_COUNTER_STEP1_TRESHOLD)
					{
						_SpeedStateCounter++;
					}
					else
					{
						_SpeedStateCounter = 0;
						_NextButtonSpeedState = STEP2;
						//_ProcessVariableIncrement = 1;
					}
					ProcessButtons(inputVariable);
					_ButtonSpeedHoldCounter = 0;
				}
				break;
			
			case STEP2:
				if (PRESS_COUNTER_TRESHOLD2 < _ButtonSpeedHoldCounter)
				{
					if (_SpeedStateCounter < PRESS_COUNTER_STEP2_TRESHOLD)
					{
						_SpeedStateCounter++;
					}
					else
					{
						_SpeedStateCounter = 0;
						_NextButtonSpeedState = STEP3;
						//_ProcessVariableIncrement = 1;
					}
					ProcessButtons(inputVariable);
					_ButtonSpeedHoldCounter = 0;
				}
				break;
				
			case STEP3:
				if (PRESS_COUNTER_TRESHOLD3 < _ButtonSpeedHoldCounter)
				{
					if (_SpeedStateCounter < PRESS_COUNTER_STEP3_TRESHOLD)
					{
						_SpeedStateCounter++;
					}
					else
					{
						_SpeedStateCounter = 0;
						_NextButtonSpeedState = STEP4;
						//_ProcessVariableIncrement = 1;
					}
					ProcessButtons(inputVariable);
					_ButtonSpeedHoldCounter = 0;
				}
				break;
			
			case STEP4:
				if (PRESS_COUNTER_TRESHOLD4 < _ButtonSpeedHoldCounter)
				{
					if (_SpeedStateCounter < PRESS_COUNTER_STEP4_TRESHOLD)
					{
						_SpeedStateCounter++;
					}
					else
					{
						_SpeedStateCounter = 0;
					}
					ProcessButtons(inputVariable);
					_ButtonSpeedHoldCounter = 0;
				}
				break;
				
			default:
				break;
		}
		
	}
	else
	{
		_ButtonSpeedHoldCounter = 0;
		_NextButtonSpeedState = STEP1;
		_ProcessVariableIncrement = INCREMENT_DEFAULT;
	}
	_CurrentButtonSpeedState = _NextButtonSpeedState;
}

uint8_t PE1MEW_RotorSteering::getButtons(void)
{
	uint8_t inputVariable = 0x00;
		
	// Test for inputs
	if (digitalRead(SW2_PIN) == HIGH)
	{
		inputVariable |= BUTTON_1;
	}
		
	if (digitalRead(SW1_PIN) == HIGH)
	{
		inputVariable |= BUTTON_2;
	}
		
	return inputVariable;
}

void PE1MEW_RotorSteering::ProcessButtons(uint8_t inputVariable)
{
	int16_t processVariable = _NextDirection;
	
	// Process input results
	switch (inputVariable)
	{
		case BUTTON_1:
			processVariable += _ProcessVariableIncrement;
			break;
		
		case BUTTON_2:
			processVariable -= _ProcessVariableIncrement;
			break;
		
		default:
			break;
	}
	
	// validation check of input
	_NextDirection = checkDegreeResult(processVariable);
}
int16_t PE1MEW_RotorSteering::checkDegreeResult(int16_t degrees)
{
	if (degrees < 0)
	{
		degrees = 0;
	}
	if (360 <= degrees)
	{
		degrees = 360;
	}
	return degrees;
}