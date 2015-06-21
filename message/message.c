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
 * message.c
 *
 * Created: 02.12.2014 22:28:01
 *  Author: Julian Iseringhausen <julian.iseringhausen@gmail.com>
 */ 

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef AVRSNIPPETS_USE_MESSAGE

#include "message.h"

void message_init(message_t* message, volatile uint8_t* buffer, uint8_t size)
{
	message->read = 0;
	message->write = 0;
	message->size = size;
	message->data = buffer;
}

#endif /* AVRSNIPPETS_USE_MESSAGE */
