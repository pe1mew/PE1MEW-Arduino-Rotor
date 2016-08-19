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

 /// \file pe1mew_rotorcontrol.h
 /// \brief Rotor class for PE1MEW Arduino Rotor Controller
 /// \date 20-7-2016
 /// \author Remko Welling (PE1MEW)
 /// \version 1.0

#ifndef PE1MEW_ROTORCONTROLCHANNELMASTER_H
#define PE1MEW_ROTORCONTROLCHANNELMASTER_H

#include <stdint.h>

#include "pe1mew_rotorcontrol.h"

static uint8_t RELAY_REST   = 0x01;	///< set bit
static uint8_t RELAY_ACTIVE = 0x00;	///< reset bit

static uint8_t REL1_PIN	= 4;		///< Pin to which the relay 1 is connected.
static uint8_t REL2_PIN	= 5;		///< Pin to which the relay 2 is connected.

#define TOTALDEGREES	360.0		///< Total number of degrees in on a compass card.
// \todo move define to static within scope of class?

/// \brief status of rotor control
enum eState { IDLE = 0, 	///< motor is not running, current Direction == next Direction
			  CW, 			///< motor is running clock wise (west-bound)
			  CCW };		///< motor is running counter clock wise (east-bound)

/// \class Rotor control class.
class PE1MEW_RotorControl
{
public:
	/// \brief Default constructor
    PE1MEW_RotorControl(void);

	/// \brief at sys tick executed function for housekeeping of the Rotor control.
	void Process(void);

    /// \brief set direction of the rotor
	/// This function is used at :
	/// - Power-up of the Rotor Controller to set last known direction (retrieved form EEprom)
	/// - During calibration process
	/// \todo verify usage
	/// \todo reconsider naming of function.
	/// \value direction in the degrees (0-360)
	/// \return direction set
	uint16_t setDirection(uint16_t direction);
	
	/// \brief get current (actual) direction of the rotor.
	/// \return current direction registered by rotor control
    uint16_t getDirection(void){return (uint16_t)_CurrentDirection;}
	
	/// \brief  get state of rotor
	/// \return true = running, false = stop.
    bool getIsRotorRunning(void){return _RotatingState;}

	/// \brief initialize angle and runtime
	/// Overloaded function form the private function Initialize()
	/// This function is used to initialize the rotor control with settings from memory
	/// Also this function is used to configure the class with new runtime settings.
	/// \value direction of the antenna. This can be the last known direction stored in memory.
	/// \value the time it takes to turn the antenna 360 degrees.
	void Initialize(uint16_t angle, uint16_t runtime);

	/// \brief function to start a timer to measure the time it takes to turn the antenna 360 degrees
	/// This function is used by the test and calibration mode.
	void calibrateRunTimeCounter(void);

	/// \brief get the result of the timer used to calibrate the rotor
	/// \return time it takes to turn the antenna 360 degrees round.
	uint16_t getRunTimeCounter(void);
	
	/// \brief Test function to show rainbow on leds
	void displayTest(void);

	/// \brief used by processCCWState() and processCWState() to execute required actions
	/// \value direction to rotate
	void setRotorTurn(eState direction);
	
	/// \brief used by processIDLEState() to stop the rotor turning
	void setRotorStop(void);

	
private:
    uint8_t  _CurrentState;			///< Current state of the state machine that keeps track of the rotor
    uint8_t  _NextState;			///< Next state the state machine will have
    float	 _CurrentDirection;		///< Current or actual direction of rotor
    uint16_t _NextDirection;        ///< Value with direction where rotor shall rotate to
	uint16_t _RunTime;				///< Value to store time required to rotate from 0 to 360 degrees.
	uint16_t _RunTimeCounter;		///< Value for counting time while calibrating
	bool	 _CalibratingMode;		///< Value to indicate calibration process is running
	float    _degreesPerTick;		///< Degrees that the antenna is turned during the time between two sys ticks.
    bool     _RotatingState;		///< Indicator to tell if the rotor is running (true) or not (false)
    eState   _RotatingDirection;	///< Status of the state machine of the rotor to keep track of the direction see eState enum

	/// \brief helper function to initialize variables en calculate values for these variables in the constructor of the classes.
	/// Functions are called that cannot be handled by the C++ default initializers
	void Initialize(void);

    /// \brief function that contains activities that shall be executed at each sys tick when the _RotatingState is IDLE.
	/// See the function implementation for a description
	void processIDLEState(void);

    /// \brief function that contains activities that shall be executed at each sys tick when the _RotatingState is CW.
	/// See the function implementation for a description
    void processCWState(void);

    /// \brief function that contains activities that shall be executed at each sys tick when the _RotatingState is CCW.
	/// See the function implementation for a description
    void processCCWState(void);

	/// \brief function to be executed in CW or CCW state when calibrating ie executed 
	/// when _CalibratingMode = true the timer counter is incremented.
	void ProcessCalibration(void);

	/// \brief set relay 1
	/// value true = run; relay active, false = stop; releay released.
	void setRotateState(bool state);

	/// \brief set relay 2
	/// value CW; relay active, CCW; releay released.
	void setRotateDirection(eState direction);

};

#endif // PE1MEW_ROTORCONTROLCHANNELMASTER_H
