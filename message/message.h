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
 *  Author: Julian Iseringhausen <julian.iseringhausen@gmail.com>
 */ 


#ifndef MESSAGE_H_
#define MESSAGE_H_

#ifdef AVRSNIPPETS_USE_MESSAGE

#include <stdint.h>
#include <avr/pgmspace.h>

typedef struct
{
	uint8_t read;
	uint8_t write;
	uint8_t size;
	volatile uint8_t* data;
}message_t;

extern void message_init(message_t* message, volatile uint8_t* buffer, uint8_t size);

static inline uint8_t message_has_data(const message_t* message) __attribute__((always_inline));
static inline uint8_t message_has_data(const message_t* message)
{
	return message->write - message->read;
}

static inline uint8_t message_equal(const message_t* message, const PGM_P compare) __attribute__((always_inline));
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

static inline uint8_t message_get_unsafe(message_t* message) __attribute__((always_inline));
static inline uint8_t message_get_unsafe(message_t* message)
{
	return *(message->data + message->read++);
}

static inline uint8_t message_peek_unsafe(message_t* message) __attribute__((always_inline));
static inline uint8_t message_peek_unsafe(message_t* message)
{
	return *(message->data + message->read);
}

static inline void message_remove_front_unsafe(message_t* message) __attribute__((always_inline));
static inline void message_remove_front_unsafe(message_t* message)
{
	++message->read;
}

static inline void message_put(message_t* message, uint8_t data) __attribute__((always_inline));
static inline void message_put(message_t* message, uint8_t data)
{
	if (message->write == message->size) {
		*(message->data + message->write - 1) = 0;
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
			*(message->data + message->write - 1) = 0;
			return;
		}
		data = pgm_read_byte(&str[i++]);
		*(message->data + message->write++) = data;
	} while(data != 0);
}

static inline void message_put_array(message_t* message, uint8_t* data, uint8_t size) __attribute__((always_inline));
static inline void message_put_array(message_t* message, uint8_t* data, uint8_t size)
{
	if (size >= message->size-1)
		return;
	message->read = 0;
	message->write = size+1;
	for (uint8_t i=0; i<size; ++i)
		message->data[i] = data[i];
	message->data[size] = 0;
}


static inline void message_clear(message_t* message) __attribute__((always_inline));
static inline void message_clear(message_t* message)
{
	message->read = 0;
	message->write = 0;
}

static inline uint8_t message_finished(message_t* message) __attribute__((always_inline));
static inline uint8_t message_finished(message_t* message)
{
	if (message->write == 0) return 0;
	return (*(message->data + message->write - 1) == 0 ? 1 : 0);
}

#endif /* AVRSNIPPETS_USE_MESSAGE */

#endif /* MESSAGE_H_ */
