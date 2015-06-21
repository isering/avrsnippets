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
 * @file   seed_eeprom.h
 * @author Julian Iseringhausen <julian.iseringhausen@gmail.com>
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

#include <stdint.h>

extern uint32_t get_seed(uint32_t* eeprom_begin, uint32_t* eeprom_end);
