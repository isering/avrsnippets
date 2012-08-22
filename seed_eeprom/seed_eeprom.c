/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/*  avr-snippets - Some useful code snippets for the Atmega Microcontroller  */
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
 * @file   seed_eeprom.c
 * @author Julian Iseringhausen
 * @date   Tue Aug 21 00:35:12 2012
 * 
 * @brief  Sets a different seed each time called using solely the builtin EEPROM.
 *
 *         Maximizes the EEPROM cells lifetime.
 *         No external hardware or free analog ports required.
 *         Minimum of 8 Bytes of free EEPROM required.
 *         Not intended for cryptographic applications.
 * 
 */

#include <stdlib.h>
#include <avr/eeprom.h>
#include <avr/io.h>

#include "seed_eeprom.h"

/** 
 * @brief Creates a slope of incremental seed values in EEPROM. Thus the function generates a 
 *        new seed everytime called while minimizing the number of EEPROM write accesses per cell.
 *        
 * 
 * @param eeprom_begin Starting address of reserved EEPROM
 * @param eeprom_end Last address of reserved EEPROM. eeprom_end=eeprom_start+4*N-1, eeprom_end<=E2END.
 */
void seed_eeprom(const uint16_t eeprom_begin, const uint16_t eeprom_end)
{
  uint16_t ptr = eeprom_begin;
  uint32_t seed = eeprom_read_dword((uint32_t*)eeprom_begin);

  for (ptr=eeprom_begin+4; ptr<eeprom_end; ptr+=4)
    if (++seed != eeprom_read_dword((uint32_t*)ptr)) break;

  if (ptr >= eeprom_end) ptr = eeprom_begin;

  eeprom_write_dword((uint32_t*)ptr, seed);

  srand((uint16_t)seed);
  srandom(seed);
}
