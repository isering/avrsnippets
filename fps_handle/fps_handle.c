/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/*  avrsnippets - Some useful code snippets for the Atmega Microcontroller   */
/*  Copyright (C) 2015  Julian Iseringhausen                                 */
/*                                                                           */
/*  This program is free software: you can redistribute it and/or modify     */
/*  it under the terms of the GNU General Public License as published by     */
/*  the Free Software Foundation, either version 3 of the License, or        */
/*  (at your option) any later version.                                      */
/*                                                                           */
/*  This program is distributed in the hope that it will be useful,          */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/*  GNU General Public License for more details.                             */
/*                                                                           */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 * fps_handle.c
 *
 * Created: 30.10.2012 21:11:28
 *  Author: Julian Iseringhausen <julian.iseringhausen@gmail.com>
 *
 *  Description: Implements VSYNC using a timer.
 */ 

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef FPS
#define FPS 400
#endif

#if FPS > F_CPU
#error FPS too high
#elif F_CPU / FPS < 65536
#define FPS_PRESCALE 1
#define FPS_CS (1<<FPS_CS0)
#elif F_CPU / FPS / 8 < 65536
#define FPS_PRESCALE 8
#define FPS_CS (1<<FPS_CS1)
#elif F_CPU / FPS / 64 < 65536
#define FPS_PRESCALE 64
#define FPS_CS ((1<<FPS_CS1)|(1<<FPS_CS0))
#elif F_CPU / FPS / 256 < 65536
#define FPS_PRESCALE 256
#define FPS_CS (1<<FPS_CS2)
#elif F_CPU / FPS / 1024 < 65536
#define FPS_PRESCALE 1024
#define FPS_CS ((1<<FPS_CS2)|(1<<FPS_CS0))
#else
#error FPS too low
#endif

#define FPS_COUNTER (F_CPU/FPS/FPS_PRESCALE)

void fps_handle_init()
{
  FPS_OCR = FPS_COUNTER;
  FPS_TIMSK = (1<<OCIE1A);
  FPS_TCCRB = FPS_CS | (1<<WGM12);
}

ISR(TIMER1_COMPA_vect) __attribute__((naked));
ISR(TIMER1_COMPA_vect)
{
  asm volatile (
    "cbi %[flag], %[bit]" "\n\t"
    "reti"
    :
    : [flag] "M" (_SFR_IO_ADDR(FLAGS)),
      [bit] "M" (FLAG_FPS_UPDATE)
  );
}
