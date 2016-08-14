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

 /// \file pe1mew_rotorcontroller.cpp
 /// \brief Main Controller class for PE1MEW Arduino Rotor Controller
 /// \date 20-7-2016
 /// \author Remko Welling (PE1MEW)
 /// \version 1.0
 /// \version 1.1	Modified turn direction in initialization phase of calibration process

 #include "pe1mew_rotorcontroller.h"

PE1MEW_RotorController::PE1MEW_RotorController():
	_RunState(NORMAL),
	_Brightness(200),
	_FunctionMemory(false),
	_CurrentDirection(15),
	_NextDirection(0),
	_RotorRunning(false),
	_RunTimeCounter(36000.0),
	_TestCalibrationState(TCSINIT),
	_rainbowCycleI(0),
	_rainbowCycleJ(0),
	_SynchronizationState(SYNCINIT),
	_SetBrightnessState(SBINIT),
	_SetBrightIncrement(true),
	_MemorytestOnce(false)
{
	Initialize();
}

void PE1MEW_RotorController::Initialize(void)
{
	// Read variables from EEprom
	_RunTimeCounter = Memory.readRunTimeCounter();
	_CurrentDirection = Memory.readDirection();
	_Brightness = Memory.readBrightness();
	
	// Set Rotor
	Rotor.Initialize(_CurrentDirection, (float)_RunTimeCounter);
	Steering.initialize(_CurrentDirection);
	
	// set Display
	Display.setBrightness(_Brightness);

	// Read startup-state from buttons	
	switch (Steering.getButtons())
	{
		case BUTTON_1:
			_RunState = SYNCHRONIZE;
			break;
		
		case BUTTON_2:
			_RunState = SET_INTENSITY;
			break;
		
		case BUTTON_BOTH:
			_RunState = TEST_CALIBRATE;
			break;

		default:
			_RunState = NORMAL;
			break;
	}
}

void PE1MEW_RotorController::Process(void)
{
	switch(_RunState)
	{
		case NORMAL:
			RunNormal();
			break;
		
		case SYNCHRONIZE:
			RunSynchronize();
			break;
		
		case SET_INTENSITY:
			RunSetBrightness();
			break;
		
		case TEST_CALIBRATE:
			RunTestCalibrate();
			break;
		
		default:
			RunDebug();
			break;	
	}
}

void PE1MEW_RotorController::RunNormal(void)
{
	Rotor.Process();
	Display.Process();
	Steering.Process();
		
	_RotorRunning = Rotor.getIsRotorRunning();
	Display.setRotorRunning(_RotorRunning);

	_NextDirection = Steering.getNextDirection();		// Get target direction from steering unit
	Rotor.setDirection(_NextDirection);					// Set rotor with target direction
	_CurrentDirection = Rotor.getDirection();			// Get actual direction form rotor
	Display.setCurrentDirection(_CurrentDirection);		// Send actual direction to LED display
	Display.setNextDirection(_NextDirection);			// Send target direction to LED display

	if(_RotorRunning)
	{
		_FunctionMemory = true;
	}
	if(!_RotorRunning && _FunctionMemory)
	{
		Memory.writeDirection(_CurrentDirection);
		_FunctionMemory = false;
	}
}

void PE1MEW_RotorController::RunSetBrightness(void)
{
	switch(_SetBrightnessState)
	{
		case SBINIT:
			Display.showLedClear();
			Display.showLedColor(0,0x00ffffff);
			_Brightness = 200;
			_SetBrightnessState = SB;
			break;
		
		case SB:
			BrightnessProcess();
			break;
		
		case SBFINISH:
			// Write brightness to memory
			Memory.writeBrightness(_Brightness);
			
			// Write brightness to display to make new settings active in following RUN mode
			Display.setBrightness(_Brightness);
			_RunState = NORMAL;
		break;
		
		default:
			break;
	}
	Display.showLedBrightness(_Brightness);
}

void PE1MEW_RotorController::RunTestCalibrate(void)
{
	switch(_TestCalibrationState)
	{
		case TCSINIT:
			TCSINITProcess();
			break;
		
		case TCS1:			// Test Leds
			TCS1Process();
			break;
			
		case TCS2:			// Test Switches 1 and 2
			TCS2Process();
			break;
			
		case TCS3:			// Test relay 1 and 2
			TCS3Process();
			break;
			
		case TCS4:			// rotate Rotor CCW until stop
			TCS4Process();
			break;
			
		case TCS5:			// rotate Rotor CW until stop and measure time
			TCS5Process();
			break;
		
		case TCS6:			// rotate Rotor CCW until stop and measure time
			TCS6Process();
			break;
		
		case TCS7:			// rotate Rotor CCW until stop and measure time
			TCS7Process();
			break;
		
		case TCSFINISH:
			_RunState = NORMAL;
			break;
			
		default:
			break;
	}
}

void PE1MEW_RotorController::RunDebug(void)
{
		Display.showLedClear();
		Display.showLedColor(0,RED);
		
		// Do something every second
		if (_debugCounter == 100)
		{
			// Put someting to do here
			
			_debugCounter = 0;
		}
		_debugCounter++;
		
		// Do something once
		if (_bMemory1)
		{
			// Put someting to do here

			_bMemory1 = false;
		}
}

void PE1MEW_RotorController::BrightnessProcess(void)
{
	// Read state from buttons
	switch (Steering.getButtons())
	{
		case BUTTON_1:
			if(_SetBrightIncrement)
			{
				if (_Brightness < 254)
				{
					_Brightness++;
				}
				else
				{
					_SetBrightIncrement = false;
				}
			}
			else
			{
				if (_Brightness > 1)
				{
					_Brightness--;
				}
				else
				{
					_SetBrightIncrement = true;
				}
			}

			_FunctionMemory = true;
			break;
					
			case BUTTON_2:
			if(_FunctionMemory)
			{
				_SetBrightnessState = SBFINISH;
			}
			break;

		default:
			break;
	}
}

void PE1MEW_RotorController::TCSINITProcess(void)
{
	Display.showLedClear();
	Display.showLedBrightness(_Brightness);
	Display.showLedColor(1,BLUE);
	_TestCalibrationState = TCS1;
}

void PE1MEW_RotorController::TCS1Process(void)
{
	if(_FunctionMemory)
	{
		if (_rainbowCycleJ < LEDCOUNT)
		{
			Display.showLedColor(_rainbowCycleJ, TCS1Rainbow(((_rainbowCycleI * 256 / LEDCOUNT) + _rainbowCycleJ) & 255));
			_rainbowCycleJ++;
		}
		else
		{
			_rainbowCycleJ = 0;
		}
		_rainbowCycleI++;
	}

	// Read state from buttons
	switch (Steering.getButtons())
	{
		case BUTTON_1:
			_FunctionMemory = true;
			break;
		
		case BUTTON_BOTH:
			if(_FunctionMemory)
			{
				Display.showLedClear();
				Display.showLedColor(0,BLUE);	// Indicate testing process
				Display.showLedColor(1,GREEN);	// Indicate first test finished
				Display.showLedColor(2,BLUE);	// Indicate second test running
				Display.showLedColor(3,BLUE);	// Indicate third test running
				_TestCalibrationState++;
				_FunctionMemory = false;
			}
			break;
		
		default:
			break;
	}	
}

/*
Input a value 0 to 255 to get a color value.
The colours are a transition r - g - b - back to r.
copied form Aruino neopixel driver code
*/

uint32_t PE1MEW_RotorController::TCS1Rainbow(byte WheelPos) 
{
	WheelPos = 255 - WheelPos;
	if(WheelPos < 85) 
	{
		return ((uint32_t)(255 - WheelPos * 3) << 16) | ((uint32_t)0 <<  8) | WheelPos * 3;
	}
	if(WheelPos < 170) 
	{
		WheelPos -= 85;
		return ((uint32_t)0 << 16) | ((uint32_t)(WheelPos * 3) <<  8) | (255 - WheelPos * 3);
	}
	WheelPos -= 170;
	return ((uint32_t)(WheelPos * 3) << 16) | ((uint32_t)(255 - WheelPos * 3) <<  8) | 0;
}

void PE1MEW_RotorController::TCS2Process(void)
{
	// Read state from buttons
	switch (Steering.getButtons())
	{
		case BUTTON_1:
			Display.showLedColor(2,GREEN);
			_FunctionMemory = true;
			break;
		
		case BUTTON_2:
			Display.showLedColor(3,GREEN);
			_FunctionMemory = true;
			break;

		case BUTTON_NONE:
			Display.showLedColor(2,BLUE);
			Display.showLedColor(3,BLUE);
			break;
		
		case BUTTON_BOTH:
			if(_FunctionMemory)
			{
				Display.showLedColor(2,BLUE);
				Display.showLedColor(3,BLUE);
				Display.showLedColor(4,BLUE);
				Display.showLedColor(5,BLUE);
				_TestCalibrationState++;
				_FunctionMemory = false;
			}
			break;
		
		default:
			break;
	}
}

void PE1MEW_RotorController::TCS3Process(void)
{
	// Read state from buttons
	switch (Steering.getButtons())
	{
		case BUTTON_1:
			Display.showLedColor(4,GREEN);
			digitalWrite(REL1_PIN, RELAY_ACTIVE);

			_FunctionMemory = true;
			break;
		
		case BUTTON_2:
			Display.showLedColor(5,GREEN);
			digitalWrite(REL2_PIN, RELAY_ACTIVE);
		
			_FunctionMemory = true;
			break;

		case BUTTON_NONE:
			Display.showLedColor(4,BLUE);
			Display.showLedColor(5,BLUE);
			digitalWrite(REL1_PIN, RELAY_REST);
			digitalWrite(REL2_PIN, RELAY_REST);

			break;
		
		case BUTTON_BOTH:
			if(_FunctionMemory)
			{
				Display.showLedColor(4,BLUE);
				Display.showLedColor(5,BLUE);
				Display.showLedColor(6,BLUE);
				_TestCalibrationState++;
				_FunctionMemory = false;
			}
			break;
		
		default:
			break;
	}
}

void PE1MEW_RotorController::TCS4Process(void)
{
	if(_MemorytestOnce)
	{
		_MemorytestOnce = false;
		if(Memory.memoryTest())
		{
			Display.showLedColor(6,GREEN);
		}
		else
		{
			Display.showLedColor(6,RED);
		}
	}

	switch (Steering.getButtons())
	{
		case BUTTON_1:
			if(!_FunctionMemory)
			{
				_MemorytestOnce = true;
			}
			_FunctionMemory = true;
			break;
		
		case BUTTON_BOTH:
		if(_FunctionMemory)
		{
			Display.showLedColor(6,BLUE);
			Display.showLedColor(7,BLUE);
			_TestCalibrationState++;
			_FunctionMemory = false;
		}
		break;
	
	default:
		break;
	}
}

void PE1MEW_RotorController::TCS5Process(void)
{
	// Read state from buttons
	switch (Steering.getButtons())
	{
		case BUTTON_1:
			_FunctionMemory = true;
			Rotor.setRotorTurn(CW);
			Display.showLedColor(7,RED);
		break;
		
		case BUTTON_2:
			_FunctionMemory = true;
			Rotor.setRotorStop();
			Display.showLedColor(7,BLUE);
		break;
		
		case BUTTON_BOTH:
			if(_FunctionMemory)
			{
				Display.showLedColor(7,BLUE);
				_TestCalibrationState++;
				_FunctionMemory = false;
			}
			break;
		
		default:
			Rotor.setRotorStop();
			Display.showLedColor(7,BLUE);
			break;
	}
}

void PE1MEW_RotorController::TCS6Process(void)
{
	if(_FunctionMemory)
	{
		Rotor.Process();
		Display.Process();
		Steering.Process();
		
		_CurrentDirection = Rotor.getDirection();			// Get actual direction form rotor
		Display.setCurrentDirection(_CurrentDirection);		// Send actual direction to LED display
	}
	
	// Read state from buttons
	switch (Steering.getButtons())
	{
		case BUTTON_1:
			if (!_FunctionMemory)
			{
				Rotor.calibrateRunTimeCounter();
				Rotor.Initialize(360, 36000.0);
				Rotor.setDirection(0);					// Set rotor with target direction 0
				Display.setRotorRunning(true);
			}
			_FunctionMemory = true;
			break;
		
		case BUTTON_2:
			if (_FunctionMemory)
			{
				_RunTimeCounter = (uint16_t)Rotor.getRunTimeCounter();
				Display.setRotorRunning(false);
			}
			break;
		
		case BUTTON_BOTH:
			if(_FunctionMemory)
			{
				// write to memory
				Memory.writeRunTimeCounter(_RunTimeCounter);
				// Configure Rotor
				Rotor.Initialize(1, _RunTimeCounter);
				Steering.initialize(0);
				//Rotor.setRotorStop();
				
				Display.showLedClear();
				Display.showLedColor(0,BLUE);	// Indicate testing process
				Display.showLedColor(1,BLUE);	// Indicate first test finished
				Display.showLedColor(2,BLUE);	// Indicate second test running
				Display.showLedColor(3,BLUE);	// Indicate third test running
				Display.showLedColor(4,BLUE);
				Display.showLedColor(5,BLUE);
				Display.showLedColor(6,BLUE);
				Display.showLedColor(7,GREEN);
				_TestCalibrationState++;
				_FunctionMemory = false;
			}
			break;
			
		default:
			break;
	}
}

void PE1MEW_RotorController::TCS7Process(void)
{
	// Read state from buttons
	switch (Steering.getButtons())
	{
		case BUTTON_1:
			_FunctionMemory = true;
			break;
			
		case BUTTON_2:
			_FunctionMemory = true;
			break;

		case BUTTON_NONE:
			if(_FunctionMemory)
			{
				_TestCalibrationState++;
				_FunctionMemory = false;
			}
			break;
			
		default:
			break;
	}
}

void PE1MEW_RotorController::RunSynchronize(void)
{
	switch(_SynchronizationState)
	{
		case SYNCINIT:
			_SynchronizationState++;
			break;
			
		case SYNC1:			
			SyncStep1();
			break;
			
		case SYNC2:			
			SyncStep2();
			break;
			
		case SYNCFINISH:
			Steering.initialize(_NextDirection);
			_RunState = NORMAL;
			break;
		
		default:
			break;
	}
}

void PE1MEW_RotorController::SyncStep1(void)
{
	if(_FunctionMemory)
	{
		Rotor.Process();
		Display.Process();
		Steering.Process();
		
		Display.setRotorRunning(false);
		
		Rotor.setDirection(_NextDirection);					// Set rotor with target direction
		_CurrentDirection = Rotor.getDirection();			// Get actual direction form rotor
		Display.setCurrentDirection(_CurrentDirection);		// Send actual direction to LED display
	}
	
	// Read state from buttons
	switch (Steering.getButtons())
	{
		case BUTTON_NONE:
			_FunctionMemory = true;
			_NextDirection = 0;								// Set CCW to 0
			break;
			
		case BUTTON_BOTH:
			_SynchronizationState=SYNCFINISH;
			_FunctionMemory = false;
			break;
		
		default:
			if(_FunctionMemory && !Rotor.getIsRotorRunning())
			{
				_SynchronizationState++;
				_FunctionMemory = false;
			}
			if(!_FunctionMemory)
			{
				Display.showLedClear();
				Display.showLedBrightness(_Brightness);
				Display.showLedColor(0,BLUE);	// Indicate testing process
			}
			break;
	}
}

void PE1MEW_RotorController::SyncStep2(void)
{
	if(_FunctionMemory)
	{
		Rotor.Process();
		Display.Process();
		Steering.Process();
		
		Display.setRotorRunning(false);
		
		Rotor.setDirection(_NextDirection);					// Set rotor with target direction
		_CurrentDirection = Rotor.getDirection();			// Get actual direction form rotor
		Display.setCurrentDirection(_CurrentDirection);		// Send actual direction to LED display
	}
	
	// Read state from buttons
	switch (Steering.getButtons())
	{
		case BUTTON_NONE:
		_FunctionMemory = true;
		_NextDirection = 360;								// Set CCW to 0
		
		break;
		
		default:
		if(_FunctionMemory && !Rotor.getIsRotorRunning())
		{
			_SynchronizationState++;
			_FunctionMemory = false;
		}
		break;
	}
}