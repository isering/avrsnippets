/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/*  avrsnippets - Some useful code snippets for the Atmega Microcontroller   */
/*  Copyright (C) 2012  Julian Iseringhausen                                 */
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

/**
 * @file   random.c
 * @author Julian Iseringhausen
 * @date   Wed Oct 24 23:10:35 2012
 * 
 * @brief  Lightweight random number generators.
 * 
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef AVRSNIPPETS_USE_RANDOM

#include <avr/io.h>

#include "avrsnippets/seed_eeprom/seed_eeprom.h"
#include "avrsnippets/random/random.h"

uint32_t random_val;

void random_init()
{
  random_val = get_seed(RANDOM_EEPROM_BEGIN, (uint32_t*)RANDOM_EEPROM_END);
  for (uint8_t i=0; i<10; ++i)
    random_xorshift32();
}

uint32_t random_xorshift32()
{
  random_val ^= random_val << 13;
  random_val ^= random_val >> 17;
  random_val ^= random_val << 5;
  return random_val;
}

uint8_t random_variate(uint8_t base, uint8_t bottom, uint8_t top, uint8_t var_bits)
{
  int16_t random = (int16_t)base + (int8_t)(random_shift_8(var_bits) & (_BV(var_bits)-1)) - (int8_t)(_BV(var_bits-1) - 1);
  
  if (random < (int16_t)bottom) random = bottom;
  else if (random > top) random = top;
  
  return (uint8_t)random;
}

#endif /* AVRSNIPPETS_USE_RANDOM */
