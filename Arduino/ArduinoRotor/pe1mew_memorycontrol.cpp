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

 /// \file pe1mew_memorycontrol.cpp
 /// \brief Memory class for PE1MEW Arduino Rotor Controller
 /// \date 20-7-2016
 /// \author Remko Welling (PE1MEW)
 /// \version 1.0	Initial version
 /// \version 1.1	Corrected readRunTimeCounter() and writeRunTimeCounter() writing to big sizes.
 
 
/*

	0	_memoryState
	1	_memBrightness
	2	_memDirection MSB
	3	_memDirection LSB
	4	..
	5	..
	6	_memRunTimeCounter MSB
	7	_memRunTimeCounter LSB

*/


#include "PE1MEW_MemoryControl.h"

#include "Arduino.h"

static uint8_t MEMORYINITIALIZED = 0x01;		///< Value indicates that memory is initialized.

PE1MEW_MemoryControl::PE1MEW_MemoryControl():
	_memRunTimeCounter(0),
	_memDirection(0),
	_memBrightness(MEMORYINITIALIZED)
{
	#ifdef FIRSTSTART							///< Test for EEProm to be initialized.
		EEPROM.write(0,MEMORYINITIALIZED);
	#endif
		
	if(EEPROM.read(0) != MEMORYINITIALIZED)
	{
		writeRunTimeCounter(36000);
		writeBrightness(200);
		writeDirection(100);
		EEPROM.update(0, MEMORYINITIALIZED);
	}
}

uint16_t PE1MEW_MemoryControl::readRunTimeCounter(void)
{
	uint16_t returnValue = 0;
	returnValue = (uint16_t)EEPROM.read(7);
	returnValue += (uint16_t)(EEPROM.read(6) << 8);
	//returnValue += (uint16_t)(EEPROM.read(5) << 16);
	//returnValue += (uint16_t)(EEPROM.read(4) << 24);
	return returnValue;
}

void PE1MEW_MemoryControl::writeRunTimeCounter(uint16_t counterValue)
{
	EEPROM.update(7, (uint8_t)counterValue);
	EEPROM.update(6, (uint8_t)(counterValue >> 8));
	//EEPROM.update(5, (uint8_t)(counterValue >> 16));
	//EEPROM.update(4, (uint8_t)(counterValue >> 24));
}

uint8_t PE1MEW_MemoryControl::readBrightness(void)
{
	return EEPROM.read(1);
}

void PE1MEW_MemoryControl::writeBrightness(uint8_t brightness)
{
	EEPROM.update(1, brightness);
}

uint16_t PE1MEW_MemoryControl::readDirection(void)
{
	uint16_t returnValue = 0;
	returnValue = (uint16_t)EEPROM.read(3);
	returnValue += (uint16_t)(EEPROM.read(2) << 8);
	if(360 < returnValue)
	{
		returnValue = 360;	
	}
	return returnValue;
}

void PE1MEW_MemoryControl::writeDirection(uint16_t direction)
{
	EEPROM.update(3, (uint8_t)direction);
	EEPROM.update(2, (uint8_t)(direction >> 8));
}

bool PE1MEW_MemoryControl::memoryTest(void)
{
	
	bool returnValue = true;
	
	// --- Test RunTimeCounter memory
	_memRunTimeCounter = readRunTimeCounter();	///< get current value and write to temporary memory
	
	writeRunTimeCounter(0xFFFF);				///< write all bits of memory
	if(readRunTimeCounter() != 0xFFFF)			///< Verify if all bits have been written
	{
		returnValue = false;								///< set result successful
	}
	
	writeRunTimeCounter(_memRunTimeCounter);
	if(readRunTimeCounter() != _memRunTimeCounter)
	{
		returnValue = false;
	}
	
	// --- Test Direction memory
	_memDirection = readDirection();
	
	writeDirection(0xFF);
	if(readDirection() != 0xFF)
	{
		returnValue = false;
	}
	
	writeDirection(_memDirection);
	if(readDirection() != _memDirection)
	{
		returnValue = false;
	}
	
	// --- Test Brightness memory
	_memBrightness = readBrightness();
	
	writeBrightness(0xFF);
	if(readBrightness() != 0xFF)
	{
		returnValue = false;
	}
	
	writeBrightness(_memBrightness);
	if(readBrightness() != _memBrightness)
	{
		returnValue = false;
	}
	
	return returnValue;
}

/*

addr = addr + 1;
if (addr == EEPROM.length()) {
	addr = 0;
}

*/

/***
    Need to divide by 4 because analog inputs range from
    0 to 1023 and each byte of the EEPROM can only hold a
    value from 0 to 255.
  ***/