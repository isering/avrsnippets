 /*
  * pixel_frame
  * Copyright (C) 2013  Julian Iseringhausen <julian.iseringhausen@gmail.com>
  *
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
  */

/*
 * fps_handle.c
 *
 * Created: 30.10.2012 21:11:28
 *  Author: Julian Iseringhausen <julian.iseringhausen@gmail.com>
 *
 *  Description: Implements VSYNC.
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
#elif F_CPU / FPS < 256
#define FPS_PRESCALE 1
#define FPS_CS (1<<CS20)
#elif F_CPU / FPS / 8 < 256
#define FPS_PRESCALE 8
#define FPS_CS (1<<CS21)
#elif F_CPU / FPS / 32 < 256
#define FPS_PRESCALE 32 
#define FPS_CS ((1<<CS21)|(1<<CS20))
#elif F_CPU / FPS / 64 < 256
#define FPS_PRESCALE 64
#define FPS_CS (1<<CS22)
#elif F_CPU / FPS / 128 < 256
#define FPS_PRESCALE 128
#define FPS_CS ((1<<CS22)|(1<<CS20))
#elif F_CPU / FPS / 256 < 256
#define FPS_PRESCALE 256
#define FPS_CS ((1<<CS22)|(1<<CS21))
#elif F_CPU / FPS / 1024 < 256
#define FPS_PRESCALE 1024
#define FPS_CS ((1<<CS22)|(1<<CS21)|(1<<CS20))
#else
#error FPS too low
#endif

#define FPS_COUNTER (F_CPU/FPS/FPS_PRESCALE)

void fps_handle_init()
{
  OCR2A = FPS_COUNTER;
  TIMSK2 = (1<<OCIE2A);
  TCCR2A = (1<<WGM21);
  TCCR2B = FPS_CS;
}

ISR(TIMER2_COMPA_vect) __attribute__((naked));
ISR(TIMER2_COMPA_vect)
{
  asm volatile (
    "cbi %[flag], %[bit]" "\n\t"
    "reti"
    :
    : [flag] "M" (_SFR_IO_ADDR(FLAGS)),
      [bit] "M" (FLAG_FPS_UPDATE)
  );
}