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

 /// \file pe1mew_memorycontrol.h
 /// \brief Memory class for PE1MEW Arduino Rotor Controller
 /// \date 20-7-2016
 /// \author Remko Welling (PE1MEW)
 /// \version 1.0
 
#ifndef PE1MEW_MEMORYCONTROL_H
#define PE1MEW_MEMORYCONTROL_H

#include <EEPROM.h>

/// \brief comment on first programming
///		As the EEProm memory is in undefined state the first time this code is programmed in to the microprocessor
///		The EEProm memory has to be formatted with default values.
///		The process is to program the microprocessor 2 times:
///		1 - uncomment the define FIRSTSTART. This will write the memory state byte so the constructor will write default values to EEProm
///		2 - program the microprocessor, EEProm will be set-up
///		3 - Comment the define FIRSTSTART. Now the constructor will test is memory is configured.
///		4 - program the microprocessor, EEPROm configuration is ready

//#define FIRSTSTART


/// \class Rotor steering class
class PE1MEW_MemoryControl
{
public:
	/// \brief Default constructor
    PE1MEW_MemoryControl();
	
	/// \brief read TimerCounter calibration value from EEProm
	/// \return timer calibration value
	uint16_t readRunTimeCounter(void);
	
	/// \brief write TimerCounter calibration value to EEProm
	/// \value timer calibration value
	void writeRunTimeCounter(uint16_t counterValue);
	
	/// \brief read brightness setting from EEProm
	/// \return brightness value (0-255)
	uint8_t readBrightness(void);
	
	/// \brief write brightness setting to EEProm
	/// \value brightness value (0-255)
	void writeBrightness(uint8_t brightness);

	/// \brief read last direction before power off from EEProm
	/// \return direction in degrees
	uint16_t readDirection(void);
	
	/// \brief write direction to EEProm
	/// \value direction in degrees
	void writeDirection(uint16_t direction);
	
	/// \brief memory test function
	/// \todo make function.
	/// tests memory operation for all stored variables
	/// \return true=success, false=failed.
	bool memoryTest(void);
	
private:

	uint8_t  _memRunTimeCounter;	///< variable for temporary storage while testing memory functions
	uint16_t _memDirection;			///< variable for temporary storage while testing memory functions
	uint8_t	 _memBrightness;		///< variable for temporary storage while testing memory functions
	
	
	/** the current address in the EEPROM (i.e. which byte we're going to write to next) **/
//	int _addr;



};

#endif // PE1MEW_MEMORYCONTROL_H
