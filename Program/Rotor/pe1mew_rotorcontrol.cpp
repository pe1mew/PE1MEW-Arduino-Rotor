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

 /// \file pe1mew_rotorcontrol.cpp
 /// \brief Rotor class for PE1MEW Arduino Rotor Controller
 /// \date 20-7-2016
 /// \author Remko Welling (PE1MEW)
 /// \version 1.0

 #include "pe1mew_rotorcontrol.h"

#include <Arduino.h>

PE1MEW_RotorControl::PE1MEW_RotorControl(void)
    :
    _CurrentState(IDLE),
    _NextState(IDLE),
    _CurrentDirection(0.0),
    _NextDirection(0),
    _RotatingState(IDLE),
    _RotatingDirection(IDLE),
	_RunTime(3600),	// 360 seconds to go 360 degrees in 10 mS steps
	_RunTimeCounter(0),
	_CalibratingMode(false)
{
	Initialize();
}

void PE1MEW_RotorControl::Initialize(void)
{
	// Configure pins for both relays
	pinMode(REL1_PIN, OUTPUT);
	pinMode(REL2_PIN, OUTPUT);
	// Set relays in to rest position.
	digitalWrite(REL1_PIN, RELAY_REST);
	digitalWrite(REL2_PIN, RELAY_REST);
	
	_degreesPerTick = TOTALDEGREES / _RunTime;
}

void PE1MEW_RotorControl::Initialize(uint16_t angle, float runtime)
{
	_CurrentDirection = (float)angle;
	_NextDirection = angle;
	_RunTime = runtime;
	//debugPrint(runtime);
	_degreesPerTick = TOTALDEGREES / _RunTime;
}

	
uint16_t PE1MEW_RotorControl::setDirection(uint16_t direction)
{
    if ((float)direction > TOTALDEGREES)
    {
        _NextDirection = TOTALDEGREES;
    }
    else
    {
        _NextDirection = direction;
    }
    return _NextDirection;
}

void PE1MEW_RotorControl::Process(void)
{
    switch(_CurrentState)
    {
    case IDLE:
        processIDLEState();
        break;

    case CW:
        processCWState();
        ProcessCalibration();
        break;

    case CCW:
        processCCWState();
        ProcessCalibration();
        break;

    default:
        break;
    }
    _CurrentState = _NextState;
}

void PE1MEW_RotorControl::processIDLEState(void)
{
    if (_NextDirection > _CurrentDirection)
    {
        _NextState = CW;
    }
    else if (_NextDirection < _CurrentDirection)
    {
        _NextState = CCW;
    }
    else
    {
        _NextState = IDLE;
    }
}

void PE1MEW_RotorControl::processCWState(void)
{
    if(_RotatingState != true)
    {
        setRotorTurn(CW);
    }
	
	_CurrentDirection += _degreesPerTick;	// Increment

    if (_CurrentDirection > _NextDirection)
    {
        setRotorStop();
        _NextState = IDLE;
    }
}

void PE1MEW_RotorControl::processCCWState(void)
{
    if(_RotatingState != true)
    {
        setRotorTurn(CCW);
    }

	_CurrentDirection -= _degreesPerTick;	// decrement

    if (_CurrentDirection < _NextDirection)
    {
        setRotorStop();
        _NextState = IDLE;
    }
}

void PE1MEW_RotorControl::setRotorTurn(eState direction)
{
    setRotateDirection(direction);
    setRotateState(true);
}

void PE1MEW_RotorControl::setRotorStop(void)
{
    setRotateDirection(IDLE);
    setRotateState(false);
}

void PE1MEW_RotorControl::setRotateState(bool state)
{
    _RotatingState = state;
    if (_RotatingState)
    {
        digitalWrite(REL1_PIN, RELAY_ACTIVE);
    }
    else
    {
        digitalWrite(REL1_PIN, RELAY_REST);
    }
}

void PE1MEW_RotorControl::setRotateDirection(eState direction)
{
    _RotatingDirection = direction;
    switch(_RotatingDirection)
    {
    case CW:
        digitalWrite(REL2_PIN, RELAY_ACTIVE);
    break;

    case CCW:
        digitalWrite(REL2_PIN, RELAY_REST);
    break;

    default:
        digitalWrite(REL2_PIN, RELAY_REST);
    break;
    }
}

void PE1MEW_RotorControl::calibrateRunTimeCounter(void)
{
	_CalibratingMode = true;
	_RunTimeCounter = 0;
}

float PE1MEW_RotorControl::getRunTimeCounter(void)
{
	_CalibratingMode = false;
	return _RunTimeCounter;
}

void PE1MEW_RotorControl::ProcessCalibration(void)
{
	if(_CalibratingMode)
	{
		_RunTimeCounter++;
	}
}

//
//void PE1MEW_RotorControl::debugPrint(float var)
//{
	//char tmp[16];
	//sprintf(tmp, "%03d ", (uint16_t)var);
	//Serial.print(tmp);
//}