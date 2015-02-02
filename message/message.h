/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/*  avrsnippets - Some useful code snippets for the Atmega Microcontroller   */
/*  Copyright (C) 2014  Julian Iseringhausen                                 */
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
 * message.h
 *
 * Created: 02.12.2014 21:30:45
 *  Author: Julian Iseringhausen
 */ 


#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <stdint.h>
#include <avr/pgmspace.h>

#define MESSAGE_READY 0
#define MESSAGE_COMPLETE 1
#define MESSAGE_ERR_OVERFLOW 2

typedef struct
{
	volatile uint8_t* data;
	uint8_t read;
	uint8_t write;
	uint8_t size;
	volatile uint8_t status;
}message_t;

extern void message_init(message_t* message, volatile uint8_t* buffer, uint8_t size);

static inline uint8_t message_equal(const message_t* message, const PGM_P compare)
{
	for (uint8_t i=0; i<message->write; ++i) {
		const uint8_t lhs = *(message->data + i);
		const uint8_t rhs = pgm_read_byte(compare+i);
		if (lhs != rhs || rhs == 0) return 0;
	}
	if (pgm_read_byte(message->data + message->write) != 0) return 0;
	return 1;
}

static inline uint8_t message_get(message_t* message) __attribute__((always_inline));
static inline uint8_t message_get(message_t* message)
{
	if (message->read == message->write) return 0;
	return *(message->data + message->read++);
}

static inline void message_put(message_t* message, uint8_t data) __attribute__((always_inline));
static inline void message_put(message_t* message, uint8_t data)
{
	if (data == 0) {
		message->status = MESSAGE_COMPLETE;
		return;
	}
	if (message->write == message->size) {
		message->status = MESSAGE_ERR_OVERFLOW;
		return;
	}
	*(message->data + message->write++) = data;
}

static inline void message_put_str(message_t* message, PGM_P str) __attribute__((always_inline));
static inline void message_put_str(message_t* message, PGM_P str)
{
	uint8_t data;
	uint8_t i=0;
	do {
		if (message->write == message->size) {
			message->status = MESSAGE_ERR_OVERFLOW;
			return;
		}
		data = pgm_read_byte(&str + i++);
		*(message->data + message->write++) = data;
	} while(data != 0);
	message->status = MESSAGE_COMPLETE;
}

static inline void message_clear(message_t* message) __attribute__((always_inline));
static inline void message_clear(message_t* message)
{
	message->read = 0;
	message->write = 0;
	message->status = MESSAGE_READY;
}

#endif /* MESSAGE_H_ */