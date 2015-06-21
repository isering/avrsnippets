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
 * hsv.c
 *
 * Created: 21.10.2012 18:12:20
 *  Author: Julian Iseringhausen <julian.iseringhausen@gmail.com>
 *
 *  Description: HSV to RGB conversion routine and
 *               typedef for HSV representation.
 */ 

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "avrsnippets/random/random.h"
#include "hsv.h"
#include "settings.h"

/*
 * HSV to RGB conversion, using fixed point arithmetic.
 */
void hsv_to_rgb(hsv_t hsv, uint8_t* r, uint8_t* g, uint8_t* b)
{
  const uint8_t h_i = hsv.h >> 8;
  const uint8_t f = hsv.h;
  const uint16_t v = (uint16_t)hsv.v + 1;
  
  const uint8_t p = (v * (255 - hsv.s)) / 256;
  const uint8_t q = (v * ((65280 - (uint16_t)hsv.s*(uint16_t)f) / 256)) / 256;
  const uint8_t t = (v * ((65280 - (uint16_t)hsv.s*(uint16_t)(255-f))/256)) / 256;
  
  switch (h_i)
  {
    case 0:
      *r = hsv.v;
      *g = t;
      *b = p;
      break;
    case 1:
      *r = q;
      *g = hsv.v;
      *b = p;
      break;
    case 2:
      *r = p;
      *g = hsv.v;
      *b = t;
      break;
    case 3:
      *r = p;
      *g = q;
      *b = hsv.v;
    break;
    case 4:
      *r = t;
      *g = p;
      *b = hsv.v;
      break;
    case 5:
      *r = hsv.v;
      *g = p;
      *b = q;
      break;
    default:
      *r = 0;
      *g = 0;
      *b = 0;
      break;
  }
}

/*
 * Generate random HSV value
 */
hsv_t hsv_random()
{
  hsv_t hsv_rand = {random_shift_16(12), settings.sat, settings.val};
  asm (
    "andi %B[hue], 0x0f" "\n\t"
    "rjmp loop_cont%="        "\n\t"
  "loop%=: "
    "subi %B[hue], 5"    "\n\t"
  "loop_cont%=: "
    "cpi %B[hue], 6"     "\n\t"
    "brsh loop%="
  : [hue] "+d" (hsv_rand.h)
  :
  );
  return  hsv_rand;
}

/*
 * Generate random HSV value, sufficiently different
 * from the old value.
 */
hsv_t hsv_random_new(hsv_t hsv)
{
  hsv.h += + 0x100 + (random_shift_16(10) & 0x3ff);
  while (hsv.h >= 0x600) hsv.h -= 0x600;
  return hsv;
}
