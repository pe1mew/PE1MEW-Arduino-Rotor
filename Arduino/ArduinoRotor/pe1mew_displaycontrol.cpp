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

 /// \file pe1mew_displaycontrol.cpp
 /// \brief Display class for PE1MEW Arduino Rotor Controller
 /// \date 20-7-2016
 /// \author Remko Welling (PE1MEW)
 /// \version 1.0

#include "pe1mew_displaycontrol.h"

#include <math.h>

PE1MEW_DisplayControl::PE1MEW_DisplayControl():
    pixels(LEDCOUNT, PIN, NEO_GRB + NEO_KHZ800),
	_CurrentDirection(0),
    _NextDirection(0),
    _brightness(128),
	_RotorRunning(false)
{
	_ledStep = MAXDegrees / LEDCOUNT;
	
	initialize();
}

PE1MEW_DisplayControl::PE1MEW_DisplayControl(uint8_t  brightness = 255,              // Maximum
                                             uint16_t nextDirection = 0,
                                             uint16_t currentDirection = 0):
    pixels(LEDCOUNT, PIN, NEO_GRB + NEO_KHZ800),
	_CurrentDirection(currentDirection),
    _NextDirection(nextDirection),
    _brightness(brightness),
	_RotorRunning(false)
{
    _ledStep = MAXDegrees / LEDCOUNT;
	
	initialize();
}

void PE1MEW_DisplayControl::initialize(void)
{
	// This initializes the NeoPixel library.
	pixels.begin();

}

void PE1MEW_DisplayControl::Process(void)
{
	pixels.clear();
	
	if (_RotorRunning)
	{
		setLedActive();
	}
	else
	{
		setLedIdle();
	}

	
	// This sends the updated pixel color to the hardware.
	
	pixels.setBrightness(_brightness);
	pixels.show();
}

void PE1MEW_DisplayControl::setLedActive(void)
{
	uint8_t red = 0, blue = 0, green = 0;
	uint8_t redMem = 0, blueMem = 0;//, greenMem = 0;
	//uint8_t north = 0, east = 0, south = 0, west = 0;
	    
	// calculate leds for current angleleds
	for (uint8_t i = 0; i < LEDCOUNT + 1; i++)
	{
		red = ledIntensity(i, _CurrentDirection);
		blue = ledIntensity(i, _NextDirection);
		    
		//north = ledIntensity(i, 0);
		//east = ledIntensity(i, 90);
		//south = ledIntensity(i, 180);
		//west = ledIntensity(i, 270);
		//green = north + east + south + west;
		    
		if (i == 0)
		{
			redMem = red;
			blueMem = blue;
			//greenMem = green;
		}
		    
		if (i == LEDCOUNT)
		{
			// set red led at i
			red += redMem;
			blue += blueMem;
			//green += greenMem;
			    
			// pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
			pixels.setPixelColor(0, pixels.Color(red,green,blue));
		}
		else
		{
			// set red led at 0
			// pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
			pixels.setPixelColor(i, pixels.Color(red,green,blue));
			    
		}
	}
}

void PE1MEW_DisplayControl::setLedIdle(void)
{
	uint8_t red = 0, blue = 0, green = 0;
	uint8_t greenMem = 0;
	
	// calculate leds for current angleleds
	for (uint8_t i = 0; i < LEDCOUNT + 1; i++)
	{
		green = ledIntensity(i, _CurrentDirection);
		
		if (i == 0)
		{
			greenMem = green;
		}
		
		if (i == LEDCOUNT)
		{
			// set red led at i
			green += greenMem;
			
			// pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
			pixels.setPixelColor(0, pixels.Color(red,green,blue));
		}
		else
		{
			// set red led at 0
			// pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
			pixels.setPixelColor(i, pixels.Color(red,green,blue));
		}
	}
}
	
uint8_t PE1MEW_DisplayControl::ledIntensity(uint8_t led, int16_t angle)
{
    uint8_t returnValue = 0;
    float AngleNormalised = 0;
    float halfLedStep = floor(_ledStep / 2.0);

    int16_t ledAngle = led * _ledStep;
    AngleNormalised = normaliseAngle(angle);
    if (ledAngle <= angle) // OK
    {
	    if ( 0 <= (angle - ledAngle - _ledStep))
	    {
		    returnValue = 0;
	    }
	   else
	    {
		    float devider = AngleNormalised / _ledStep;
		    returnValue = (uint8_t)(255.0 * (1 - devider));
	    }
    }
    else // angle >= ledAngle
    {
	    if ( (angle - ledAngle + _ledStep) <= 0)
	    {
		    returnValue = 0;
	    }
	    else
	    {
		    float devider = AngleNormalised / _ledStep;
		    returnValue = (uint8_t)(255.0 * devider);
	    }
    }
    return returnValue;
}

int16_t PE1MEW_DisplayControl::normaliseAngle(uint16_t angle)
{
	int16_t returnValue = 0;
	returnValue = angle - (( angle / _ledStep ) * _ledStep);
	return returnValue;
}

void PE1MEW_DisplayControl::showLedClear(void)
{
	pixels.clear();
	pixels.show();
}

void PE1MEW_DisplayControl::showLedBrightness(uint8_t brightness)
{
	pixels.setBrightness(brightness);
	pixels.show();
}

void PE1MEW_DisplayControl::showLedColor(uint8_t led, uint32_t color)
{
	// pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
	pixels.setPixelColor(led, color);
	pixels.show();
}

/// \todo remove function
void PE1MEW_DisplayControl::showLed(void)
{
	
}

void PE1MEW_DisplayControl::displayTest(void)
{
	
}

/*

float           _frequency;
int             _led;
		
		
==============

_led(0),
_frequency(0.3),



uint8_t red;
uint8_t green;
uint8_t blue;

if (!_runningTest9)
{
	_led = 0;
	_runningTest9 = true;
}

// http://krazydad.com/tutorials/makecolors.php
red = sin(_frequency * _led + 0) * 127 + 128;
green = sin(_frequency * _led + 2) * 127 + 128;
blue = sin(_frequency * _led + 4) * 127 + 128;

if(_led < APA101_test.getFrameSize())
{
	APA101_test.setColor(_led, red, green, blue);
}

*/