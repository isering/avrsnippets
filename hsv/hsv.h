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
 * hsv.h
 *
 * Created: 21.10.2012 18:11:09
 *  Author: Julian Iseringhausen <julian.iseringhausen@gmail.com>
 *
 *  Description: HSV to RGB conversion routine and
 *               typedef for HSV representation.
 */ 

#ifndef HSV_H_
#define HSV_H_

typedef struct  
{
  uint16_t h;
  uint8_t s;
  uint8_t v;
} hsv_t;

extern void hsv_to_rgb(hsv_t hsv, uint8_t* r, uint8_t* g, uint8_t* b);

extern hsv_t hsv_random();
extern hsv_t hsv_random_new(hsv_t hsv);

#endif /* HSV_H_ */
