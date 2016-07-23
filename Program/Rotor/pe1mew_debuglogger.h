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

 /// \file PE1MEW_DebugLogger.h
 /// \brief debug class for PE1MEW Arduino Rotor Controller
 /// \date 20-7-2016
 /// \author Remko Welling (PE1MEW)
 /// \version 1.0
 
#ifndef PE1MEW_DebugLogger_H
#define PE1MEW_DebugLogger_H

#include <EEPROM.h>

/// \class Debug logger class
class PE1MEW_DebugLogger
{
public:
	/// \brief Default constructor
    PE1MEW_DebugLogger();

	void showLog(void);
	
private:



};

#endif // PE1MEW_DebugLogger_H
