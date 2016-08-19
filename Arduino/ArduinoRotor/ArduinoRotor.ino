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

 /// \file Sketch.h
 /// \brief PE1MEW Arduino Rotor Controller
 /// \date 20-7-2016
 /// \author Remko Welling (PE1MEW)
 /// \version 1.0

/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>
#include <avr/interrupt.h>
/*End of auto generated code by Atmel studio */

#include "Rotor/pe1mew_rotorcontroller.h"

#ifdef __AVR__
  #include <avr/power.h>
//Beginning of Auto generated function prototypes by Atmel Studio
void setup();
void loop();
//End of Auto generated function prototypes by Atmel Studio
#endif

// rotor object
PE1MEW_RotorController rotorController = PE1MEW_RotorController();

volatile bool ticked = false;
/// \brief functions called at startup to configure hardware
void setup() 
{
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  /// \brief initialize serial communication for debug activities
  /// \todo shall be removed and replaced by debug logger
  Serial.begin(115200); // initialize serial:
  
  
  /// \brief timer configuartion
  /// The contorcontroller is running at fixed interuppt intervals.
  /// The time base is 10 mS beacause the Neopixel need aproximately 2 mS to update the leds.
  /// During this update interrupts are disabled to prevent timing delay in the protocol of the 
  /// Neopixel led.
  ///
  /// In this implemtation registers in the ATMega328 are used.
  /// In the case of using other mircoprocessors than used in this example it is suggested to use the 
  /// interrupt library from Arduino.
  // initialize Timer1
  cli();          // disable global interrupts
  TCCR1A = 0;     // set entire TCCR1A register to 0
  TCCR1B = 0;     // same for TCCR1B

  // set compare match register to desired timer count:
  OCR1A = 78; // 8 MHz / 1024 sec / 100 = 1 mS interval
  // turn on CTC mode:
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler:
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS12);
  // enable timer compare interrupt:
  TIMSK1 |= (1 << OCIE1A);
  sei();          // enable global interrupts
}

/// \brief main loop
/// This loop waits until ticked = true before executing process().
void loop() 
{
  if (ticked)
  {
    rotorController.Process();
    ticked = false;
  }
}

/// \brief ISR for systick timer
/// using timer1 overflow
ISR(TIMER1_COMPA_vect) 
{
  // process the timer1 overflow here
  ticked = true;  // set trigger for mainloop to run process() function of rotorController object.
}

