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

 /// \file pe1mew_rotorcontroller.h
 /// \brief Main Controller class for PE1MEW Arduino Rotor Controller
 /// \date 20-7-2016
 /// \author Remko Welling (PE1MEW)
 /// \version 1.0

#ifndef PE1MEW_ROTORCONTROLLER_H
#define PE1MEW_ROTORCONTROLLER_H

#include "pe1mew_rotorcontrol.h"
#include "pe1mew_displaycontrol.h"
#include "pe1mew_rotorsteering.h"
#include "pe1mew_memorycontrol.h"

/// \brief states in which the rotor controller can operate.
enum eRunState { NORMAL = 0,		///< Normal operation
				 SET_INTENSITY,		///< Set brightness mode: Set intensity of LED display
				 SYNCHRONIZE,		///< Synchronize mode: synchronize rotor unit and Rotor Controller
				 TEST_CALIBRATE };	///< Test and calibration mode

/// \brief states for Test and Calibration Mode
/// \todo modify to move sync to separate enum and mode.
enum eTestCalState { TCSINIT = 0,
					 TCS1,			///< Test leds
					 TCS2,			///< Test Switches
					 TCS3,			///< Test Relays
					 TCS4,			///< Calibrate Rotor: set CCW until stop
					 TCS5,			///< Rotate CW until stop 
					 TCS6,			///< Rotate CCW until stop
					 TCS7,			///< Rotate CCW until stop
					 TCSFINISH		///< Save rotor calibration and exit to normal operation
					 };

/// \brief states for synchronising rotor controller and unit
enum eSyncState { SYNCINIT = 0,		///< Calibrate Rotor: set CCW until stop
				  SYNC1,            ///< Rotate CW until stop 
				  SYNC2,            ///< Rotate CCW until stop
				  SYNCFINISH};		///< End mode.

/// \brief states for set brightness mode
enum eSetBrightState { SBINIT = 0,	///< Make preparations for setting brightness
					   SB,			///< Set brightness
					   SBFINISH };	///< Save set brightness and exit to normal operation

/// \class PE1MEW_RotorController
/// \brief Main controller class.
/// All functions are controlled from this class.
class PE1MEW_RotorController
{
public:
    /// \brief default constructor
	PE1MEW_RotorController();
	
	/// \brief House keeping function
	/// Runs all activities that periodically have to run at sys tick interval.
	/// Activities involved are all house keeping activities that need to be run in other objects.
	/// Objects are:
	/// - Rotor controller, Controls the relays of the rotor.
	/// - Display controller, Controls the display of the rotor
	/// - Steering controller, reads buttons
	void Process(void);

private:
    PE1MEW_RotorControl Rotor = PE1MEW_RotorControl();			///< Rotor control object controls the rotor trough relays.
    PE1MEW_DisplayControl Display = PE1MEW_DisplayControl();	///< Display control object controls the Neopixel leds of the compass card
	PE1MEW_RotorSteering Steering = PE1MEW_RotorSteering();		///< Object that control the switches (buttons)
	PE1MEW_MemoryControl Memory = PE1MEW_MemoryControl();		///< Memory object for all memory operation. is not included in sys tick.

	// General variables
	uint8_t _RunState;
	uint8_t _Brightness;
	bool	_FunctionMemory;
	
	/// Normal running state variables
	uint16_t _CurrentDirection;			///< Temporary variable for the exchange between rotor control and display control
	uint16_t _NextDirection;			///< Temporary variable for the exchange between rotor control and display control
	bool	 _RotorRunning;				///< Temporary variable for the exchange between rotor control and display control
	uint16_t _RunTimeCounter;			///< Temporary variable to keep value in initialization and calibration process.

	// Test and Calibration state variables
	uint8_t  _TestCalibrationState;		///< variable to keep track of the test and calibration process.
	uint16_t _rainbowCycleI;			///< \todo revise?
	uint16_t _rainbowCycleJ;
	
	// Synchronization variables
	uint8_t _SynchronizationState;
	
	// Set brightness state variables
	uint8_t _SetBrightnessState;		///< variable to keep track of the test and set brightness process.
	bool	_SetBrightIncrement;		///< variable to keep register in- or decrement of the brightness setting.
	
	// Memory test variables
	bool _MemorytestOnce;
	
	// Debug running state variables
	int _debugCounter = 0;				///< \todo shall be removed
	bool _bMemory1 = true;				///< \todo shall be removed

	/// \brief helper function of the constructor
	/// This function is used to execute functions that cannot be held in the default initializers
	void Initialize(void);

	/// \brief all functions to be executed at sys tick interval in Normal mode
	void RunNormal(void);
	
	/// \brief all functions to be executed at sys tick interval  in set brightness mode
	void RunSetBrightness(void);
	/// \brief helper function in set brightness mode for state SB
	void BrightnessProcess(void);
		 
	/// \brief all functions to be executed at sys tick interval for synchronizing
	void RunSynchronize(void);
	void SyncStep1(void);
	void SyncStep2(void);
	
	
	/// \brief all functions to be executed at sys tick interval for test and calibration
	void RunTestCalibrate(void);
	void TCSINITProcess(void);
	void TCS1Process(void);
	uint32_t TCS1Rainbow(byte WheelPos);
	void TCS2Process(void);
	void TCS3Process(void);
	void TCS4Process(void);
	void TCS5Process(void);
	void TCS6Process(void);
	void TCS7Process(void);

	void RunDebug(void);

};
#endif // PE1MEW_ROTORCONTROLLER_H
