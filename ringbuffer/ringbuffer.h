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
 * @file   ringbuffer.h
 * @author Julian Iseringhausen <julian.iseringhausen@gmail.com>
 * @date   21.10.2012 12:13:41
 * 
 * @brief  A ring buffer.
 * 
 */
#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#include <stdint.h>

typedef struct
{
  volatile uint8_t read;
  volatile uint8_t write;
  volatile uint8_t* begin;
  uint8_t size;
}ringbuffer_t;

extern void ringbuffer_init(ringbuffer_t* r, volatile uint8_t* buffer, uint8_t size);

static inline uint8_t ringbuffer_get_wait(ringbuffer_t* r) __attribute__(( always_inline ));
static inline uint8_t ringbuffer_get_wait(ringbuffer_t* r)
{
  uint8_t temp = r->read;
  while (temp == r->write);
  uint8_t data = *(r->begin + temp);
  if (++temp == r->size) temp = 0;
  r->read = temp;
  return data;
}

static inline uint8_t ringbuffer_get_nowait(ringbuffer_t* r) __attribute__(( always_inline ));
static inline uint8_t ringbuffer_get_nowait(ringbuffer_t* r)
{
  uint8_t temp = r->read;
  if (temp == r->write) return 0;
  uint8_t data = *(r->begin + temp);
  if (++temp == r->size) temp = 0;
  r->read = temp;
  return data;
}

static inline void ringbuffer_put(ringbuffer_t* r, uint8_t data) __attribute__(( always_inline ));
static inline void ringbuffer_put(ringbuffer_t* r, uint8_t data)
{
  uint8_t temp = r->write;
  *(r->begin + temp) = data;
  if (++temp == r->size) temp = 0;
  r->write = temp;
}

static inline uint8_t ringbuffer_size(ringbuffer_t* r) __attribute__(( always_inline ));
static inline uint8_t ringbuffer_size(ringbuffer_t* r)
{
  uint8_t temp_write = r->write;
  uint8_t temp_read = r->read;
  if (temp_write >= temp_read) return temp_write - temp_read;
  else return temp_write + (r->size - temp_read);
}

#endif /* RINGBUFFER_H_ */
