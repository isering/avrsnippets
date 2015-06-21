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
 * helper.h
 *
 * Created: 21.10.2012 12:10:58
 *  Author: Julian Iseringhausen <julian.iseringhausen@gmail.com>
 */ 

#ifndef HELPER_H_
#define HELPER_H_

#include <stdint.h>

#define set(port, pin) ((port) |= (uint8_t)(1 << (pin)))
#define clear(port, pin) ((port) &= (uint8_t)~(1 << (pin)))
#define toggle(port, pin) ((port) ^= (uint8_t)(1 << (pin)))
#define get(port, pin) (((port) & (1 << (pin))) ? 1 : 0)
#define pulse(port, pin) \
  do { \
    set((port), (pin)); \
    clear((port), (pin)); \
  } while (0)


#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

#endif /* HELPER_H_ */
