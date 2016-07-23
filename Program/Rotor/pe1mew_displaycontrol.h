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

 /// \file pe1mew_displaycontrol.h
 /// \brief Display class for PE1MEW Arduino Rotor Controller
 /// \date 20-7-2016
 /// \author Remko Welling (PE1MEW)
 /// \version 1.0

// \todo move static variables within scope of class?

#ifndef PE1MEW_ROTORNEOPIXELDISPLAY_H
#define PE1MEW_ROTORNEOPIXELDISPLAY_H

#include <stdint.h>
#include "../AdaFruit/Adafruit_NeoPixel.h"

/// \brief enum for predefined colors
enum eColor { RED = 0x00FF0000, 	///< Red
			  GREEN = 0x0000FF00, 	///< Green
			  BLUE = 0x000000FF, 	///< Blue
			  OFF = 0x00000000 		///< Led is switched off.
			  };

/// Settings required for the Adafruit Neopixel ring.
// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
static uint8_t PIN = 6;				///< Pin to which the Neopixel ring is connected.

// How many NeoPixels are attached to the Arduino?
static uint8_t  LEDCOUNT   = 24;	///< Number of leds in the Neopixel ring.
static uint16_t MAXDegrees = 360;	///< Total number of degrees in on a compass card.

/// \class Display controller class
/// All functions to show information on the Neopixel ring.
/// Functions are split in to:
/// - Functions for showing directions
/// - Functions for showing Test and calibration information.
class PE1MEW_DisplayControl
{
public:
	/// \Brief Default constructor
    PE1MEW_DisplayControl();
	
	/// \brief overloaded constructor
	/// This allows to set variables at creation of the object.
	/// \value the initial brightness of the NeoPixel leds. value shall be between 0 (off) and 255 (maximum).
	/// \value the next direction in degrees to which the rotor controller shall turn. this value should be the same as the current angle.
	/// \value current direction in degrees of antenna. This value should be the last stored direction from memory.
    PE1MEW_DisplayControl(uint8_t  brightness,
                          uint16_t nextDirection,
                          uint16_t currentAngle);

	/// \brief at systick executed function for housekeeping of the Display controller.
	void Process(void);

	/// \ brief set current direction of the rotor controller.
	/// \value direction in degrees.
	void setCurrentDirection(uint16_t angle){_CurrentDirection = angle;}
	
	/// \ brief set the next direction t which the antenna shall be turned.
	/// \value direction in degrees.
    void setNextDirection(uint16_t angle){_NextDirection = angle;}
	
	/// \brief Set brightness of the NeoPixel leds. 
	/// \value brightness is a value shall between 0 (off) and 255 (maximum).
    void setBrightness(uint8_t brightness){_brightness = brightness;}
	
	/// \brief inform the display class that the motor of the controller is running
	/// In this situation nextDirection is unequal to currentDirection
	/// \value When true motor is running.
	void setRotorRunning(bool state){_RotorRunning = state;}
	
	/// \todo functions shall be revised.
	void showLedClear(void);
	void showLedBrightness(uint8_t brightness);
	void showLedColor(uint8_t led, uint32_t color);
	void showLed(void);
    
private:

	// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
	// Note that for older NeoPixel strips you might need to change the third parameter--see the strand test
	// example for more information on possible values.
	// = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
	Adafruit_NeoPixel pixels;				///< Neopixel library object to control Neopixel leds.
	
    uint16_t _CurrentDirection;             ///< Current or actual direction of rotor in degrees
    uint16_t _NextDirection;                ///< Wanted direction of rotor in degrees
    uint8_t  _brightness;					///< brightness of the Neopixel leds. 0=off 255=maximum.
	bool	 _RotorRunning;					///< state of the rotor motor. true=running, false=stop.
	uint8_t  _ledStep;						///< angle in degrees between two led of the NeoPixel ring.

	/// \brief Helper function of contructor to initialize variables.
	void	initialize(void);
	
	///\brief Displays Next- and Currentdirection on the compas card in 2 colors
	/// Next direction is in blue, current direction is red.
	/// This function is used when motor is running or at test and configuration modes
	void	setLedActive(void);
	
	/// \brief Displays Currentdirection on the compas card in green
	/// This function is used when motor is idle or at test and configuration modes
	void	setLedIdle(void);
	
	/// \brief helper function to calculate led intensity of led n relative to the direction in degrees.
	/// This function is used by setLedActive() and setLedIdle()
	/// \value lednumber ( 0 to ledcount in neopixel ring)
	/// \value angle in degrees (0-360)
	///\ return intensity of the led (0 = off, 255 - maiximal)
    uint8_t ledIntensity(uint8_t led, int16_t angle);
	
	/// \brief helper function to calculate relative angle within ledstep.
	/// This function is used by setLedActive() and setLedIdle()
	/// \value angle in degrees (0-360)
	int16_t normaliseAngle(uint16_t angle);
	
	void displayTest(void);

};

#endif // PE1MEW_ROTORNEOPIXELDISPLAY_H
