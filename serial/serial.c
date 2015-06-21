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
 * serial.c
 *
 * Created: 21.10.2012 12:08:43
 *  Author: Julian Iseringhausen <julian.iseringhausen@gmail.com>
 */ 

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef AVRSNIPPETS_USE_SERIAL

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <inttypes.h>
#include <stddef.h>

#include "avrsnippets/serial/serial.h"

#ifdef SERIAL_ENABLE_RX_INT_RINGBUFFER
volatile uint8_t serial_buffer_in[SERIAL_BUFSIZE_IN];
ringbuffer_t serial_rb_in;
volatile uint8_t serial_data_received = 0;
#endif

#ifdef SERIAL_ENABLE_TX_INT_RINGBUFFER
volatile uint8_t serial_buffer_out[SERIAL_BUFSIZE_OUT];
ringbuffer_t serial_rb_out;
#endif

#ifdef SERIAL_ENABLE_RX_INT_MESSAGE
volatile uint8_t serial_buffer_in[SERIAL_BUFSIZE_IN];
message_t serial_message_in;
#endif

#ifdef SERIAL_ENABLE_RX_INT_MESSAGE_BUFFERED
volatile uint8_t serial_buffer_in1[SERIAL_BUFSIZE_IN];
volatile uint8_t serial_buffer_in2[SERIAL_BUFSIZE_IN];
message_t serial_message_in1;
message_t serial_message_in2;
message_t* volatile serial_message_in_ptr;
message_t* serial_message_in_ptrb;
#endif

#ifdef SERIAL_ENABLE_TX_INT_MESSAGE
volatile uint8_t serial_buffer_out[SERIAL_BUFSIZE_IN];
message_t serial_message_out;
#endif

#ifdef SERIAL_ENABLE_RX_INT_RINGBUFFER
ISR(SERIAL_RX_vect) __attribute__((naked));
ISR(SERIAL_RX_vect)
{
  asm volatile (
    "push r16"	       "\n\t"
    "in r16, __SREG__" "\n\t"
    "push r16"	       "\n\t"
    "push r26"	       "\n\t"
    "push r27"	       "\n\t"
	
    "lds r26, serial_rb_in+%[offset_begin]"   "\n\t"
    "lds r27, serial_rb_in+%[offset_begin]+1" "\n\t"
    "lds r16, serial_rb_in+%[offset_write]"   "\n\t"
    "add r26, r16"			      "\n\t"
    "clr r16"				      "\n\t"
    "adc r27, r16"			      "\n\t"
    "lds r16, serial_rb_in+%[offset_write]"   "\n\t"
    "inc r16"				      "\n\t"
    "cpi r16, %[bufsize]"		      "\n\t"
    "brne no_clear%="			      "\n\t"
    "clr r16"				      "\n\t"
    "no_clear%=: "
    "sts serial_rb_in+%[offset_write], r16"   "\n\t"
    "lds r16, %[SDR]"			      "\n\t"
    "st X, r16"				      "\n\t"	

    "pop r27"		"\n\t"
    "pop r26"		"\n\t"
    "pop r16"		"\n\t"
    "out __SREG__, r16"	"\n\t"
    "pop r16"		"\n\t"
    "reti"
  : 
  : [offset_write] "M" (offsetof(ringbuffer_t, write)),
    [offset_begin] "M" (offsetof(ringbuffer_t, begin)),
    [bufsize] "M" (SERIAL_BUFSIZE_IN),
    [SDR] "m" (SERIAL_DATA_REGISTER)
  );
}
#endif /* SERIAL_ENABLE_RX_INT_RINGBUFFER */

#ifdef SERIAL_ENABLE_RX_INT_MESSAGE
ISR(SERIAL_RX_vect) __attribute__((naked));
ISR(SERIAL_RX_vect)
{
  asm volatile (
    "push r16"	       "\n\t"
    "in r16, __SREG__" "\n\t"
    "push r16"	       "\n\t"
    "push r17"         "\n\t"
    "push r26"	       "\n\t"
    "push r27"	       "\n\t"

    "lds r26, serial_message_in + %[offset_data]"   "\n\t"
    "lds r27, serial_message_in + %[offset_data]+1" "\n\t"
    "lds r16, serial_message_in + %[offset_write]"  "\n\t"
    "lds r17, serial_message_in + %[offset_size]"   "\n\t"
    "cp r16, r17"				    "\n\t"
    "brlo message_no_overflow%="		    "\n\t"

    "dec r16"                                       "\n\t"
    "add r26, r16"                                  "\n\t"
    "clr r16"                                       "\n\t"
    "adc r27, r16"                                  "\n\t"
    "st X, r16"                                     "\n\t"
    "rjmp message_end%="			    "\n\t"
		
    "message_no_overflow%=: "
    "inc r16"					    "\n\t"
    "sts serial_message_in + %[offset_write], r16"  "\n\t"
    "dec r16"					    "\n\t"
    "add r26, r16"				    "\n\t"
    "clr r16"					    "\n\t"
    "adc r27, r16"				    "\n\t"
    "lds r16, %[SDR]"				    "\n\t"
    "st X, r16"					    "\n\t"
		
    "message_end%=: "
    "pop r27"	        "\n\t"
    "pop r26"		"\n\t"
    "pop r17"           "\n\t"
    "pop r16"		"\n\t"
    "out __SREG__, r16"	"\n\t"
    "pop r16"		"\n\t"
    "reti"
    :
    : [offset_status] "M" (offsetof(message_t, status)),
      [offset_write]  "M" (offsetof(message_t, write)),
      [offset_size] "M" (offsetof(message_t, size)),
      [offset_data] "M" (offsetof(message_t, data)),
      [message_ready] "M" (MESSAGE_READY),
      [message_err_overflow] "M" (MESSAGE_ERR_OVERFLOW),
      [SDR] "m" (SERIAL_DATA_REGISTER)
  );
}
#endif /* SERIAL_ENABLE_RX_INT_MESSAGE */

#ifdef SERIAL_ENABLE_RX_INT_MESSAGE_BUFFERED
ISR(SERIAL_RX_vect) __attribute__((naked));
ISR(SERIAL_RX_vect)
{
  asm volatile (
    "push r16"	       "\n\t"
    "in r16, __SREG__" "\n\t"
    "push r16"	       "\n\t"
    "push r17"	       "\n\t"
    "push r28"	       "\n\t"
    "push r29"	       "\n\t"
    "push r30"	       "\n\t"
    "push r31"	       "\n\t"
	
    "ldi r30, lo8(%[message_pb])" "\n\t"
    "ldi r31, hi8(%[message_pb])" "\n\t"
    "ld r28, Z+"		  "\n\t"
    "ld r29, Z"		          "\n\t"

    "ldd r30, Y+%[offset_data]"   "\n\t"
    "ldd r31, Y+%[offset_data]+1" "\n\t"
    "ldd r16, Y+%[offset_write]"  "\n\t"
    "ldd r17, Y+%[offset_size]"   "\n\t"
    "cp r16, r17"		  "\n\t"
    "brlo message_no_overflow%="  "\n\t"

    "dec r16"		          "\n\t"
    "add r30, r16"		  "\n\t"
    "clr r16"		          "\n\t"
    "adc r31, r16"		  "\n\t"
    "st Z, r16"                   "\n\t"
    "rjmp message_complete%="	  "\n\t"
	
    "message_no_overflow%=: "
    "inc r16"			  "\n\t"
    "std Y+%[offset_write], r16"  "\n\t"
    "dec r16"			  "\n\t"
    "add r30, r16"		  "\n\t"
    "clr r16"			  "\n\t"
    "adc r31, r16"		  "\n\t"
    "lds r16, %[SDR]"		  "\n\t"
    "st Z, r16"			  "\n\t"
    "tst r16"			  "\n\t"
    "breq message_complete%="	  "\n\t"
    "rjmp message_end%="	  "\n\t"
		
    "message_complete%=: "
    "ldi r30, lo8(%[message_p])"  "\n\t"
    "ldi r31, hi8(%[message_p])"  "\n\t"
    "ld r16, Z+"		  "\n\t"
    "ld r17, Z+"		  "\n\t"
    "st -Z, r29"		  "\n\t"
    "st -Z, r28"		  "\n\t"
    "ldi r28, lo8(%[message_pb])" "\n\t"
    "ldi r29, hi8(%[message_pb])" "\n\t"
    "st Y+, r16"		  "\n\t"
    "st Y, r17"			  "\n\t"
		
    "message_end%=: "
    "pop r31"	        "\n\t"
    "pop r30"	        "\n\t"
    "pop r29"	        "\n\t"
    "pop r28"		"\n\t"
    "pop r17"		"\n\t"
    "pop r16"		"\n\t"
    "out __SREG__, r16"	"\n\t"
    "pop r16"	        "\n\t"
    "reti"
    :
    : [message_p] "m" (serial_message_in_ptr),
      [message_pb] "m" (serial_message_in_ptrb),
      [offset_write]  "M" (offsetof(message_t, write)),
      [offset_data] "M" (offsetof(message_t, data)),
      [offset_size] "M" (offsetof(message_t, size)),
      [SDR] "m" (SERIAL_DATA_REGISTER)
  );
}
#endif /* SERIAL_ENABLE_RX_INT_MESSAGE_BUFFERED */

#ifdef SERIAL_ENABLE_TX_INT_RINGBUFFER
ISR(SERIAL_UDRE_vect)
{
  if (serial_rb_out.read != serial_rb_out.write)
  SERIAL_DATA_REGISTER = ringbuffer_get_nowait(&serial_rb_out);
  else
  clear(SERIAL_DRE_INT_CONTROL , SERIAL_DRE_INT);
}

void serial_send(uint8_t data)
{
  // Wait for empty transmit buffer
  ringbuffer_put(&serial_rb_out, data);
  set(SERIAL_DRE_INT_CONTROL, SERIAL_DRE_INT);
}

void serial_send_str(PGM_P str)
{
	uint8_t data;
	uint8_t i=0;
	do {
		data = pgm_read_byte(&str[i++]);
		ringbuffer_put(&serial_rb_out, data);
	} while(data != 0);
	set(SERIAL_DRE_INT_CONTROL, SERIAL_DRE_INT);
}

void serial_send_array(uint8_t* data, uint8_t size)
{
	for (uint8_t i=0; i<size; ++i)
		ringbuffer_put(&serial_rb_out, data[i]);
	set(SERIAL_DRE_INT_CONTROL, SERIAL_DRE_INT);
}
#endif /* SERIAL_ENABLE_TX_INT_RINGBUFFER */

#ifdef SERIAL_ENABLE_TX_INT_MESSAGE
ISR(SERIAL_UDRE_vect)
{
	const uint8_t data = message_get(&serial_message_out);
	SERIAL_DATA_REGISTER = data;
	if (data == 0) clear(SERIAL_DRE_INT_CONTROL, SERIAL_DRE_INT);
}

void serial_send(uint8_t data)
{
	message_put(&serial_message_out, data);
	set(SERIAL_DRE_INT_CONTROL, SERIAL_DRE_INT);
}

void serial_send_str(PGM_P str)
{
	message_clear(&serial_message_out);
	message_put_str(&serial_message_out, str);
	set(SERIAL_DRE_INT_CONTROL, SERIAL_DRE_INT);
}

void serial_send_array(uint8_t* data, uint8_t size)
{
	message_put_array(&serial_message_out, data, size);
	set(SERIAL_DRE_INT_CONTROL, SERIAL_DRE_INT);
}
#endif /* SERIAL_ENABLE_TX_INT_MESSAGE */

void serial_init()
{
  // Set baud rate
  SERIAL_BAUDRATE_REGISTER_H = (uint8_t)(SERIAL_UBRR_VAL >> 8);
  SERIAL_BAUDRATE_REGISTER_L = (uint8_t)(SERIAL_UBRR_VAL);
  
  #if SERIAL_CHARSIZE == 5
  SERIAL_CHARSIZE_CONTROL_1 = SERIAL_CHARSIZE_5BIT_1;
  SERIAL_CHARSIZE_CONTROL_2 = SERIAL_CHARSIZE_5BIT_2;
  #elif SERIAL_CHARSIZE == 6
  SERIAL_CHARSIZE_CONTROL_1 = SERIAL_CHARSIZE_6BIT_1;
  SERIAL_CHARSIZE_CONTROL_2 = SERIAL_CHARSIZE_6BIT_2;
  #elif SERIAL_CHARSIZE == 7
  SERIAL_CHARSIZE_CONTROL_1 = SERIAL_CHARSIZE_7BIT_1;
  SERIAL_CHARSIZE_CONTROL_2 = SERIAL_CHARSIZE_7BIT_2;
  #elif SERIAL_CHARSIZE == 8
  SERIAL_CHARSIZE_CONTROL_1 = SERIAL_CHARSIZE_8BIT_1;
  SERIAL_CHARSIZE_CONTROL_2 = SERIAL_CHARSIZE_8BIT_2;
  #elif SERIAL_CHARSIZE == 9
  SERIAL_CHARSIZE_CONTROL_1 = SERIAL_CHARSIZE_9BIT_1;
  SERIAL_CHARSIZE_CONTROL_2 = SERIAL_CHARSIZE_9BIT_2;
  #endif
  
  #ifdef SERIAL_ENABLE_DOUBLE_SPEED
  set(SERIAL_U2X_CONTROL, SERIAL_U2X);
  #endif

  #if defined(SERIAL_ENABLE_RX_INT_RINGBUFFER)
  ringbuffer_init(&serial_rb_in, serial_buffer_in, SERIAL_BUFSIZE_IN);
  #elif defined(SERIAL_ENABLE_RX_INT_MESSAGE)
  message_init(&serial_message_in, serial_buffer_in, SERIAL_BUFSIZE_IN);
  #elif defined(SERIAL_ENABLE_RX_INT_MESSAGE_BUFFERED)
  message_init(&serial_message_in1, serial_buffer_in1, SERIAL_BUFSIZE_IN);
  message_init(&serial_message_in2, serial_buffer_in2, SERIAL_BUFSIZE_IN);
  serial_message_in_ptr = &serial_message_in1;
  serial_message_in_ptrb = &serial_message_in2;
  #endif
  
  #if defined(SERIAL_ENABLE_TX_INT_RINGBUFFER)
  ringbuffer_init(&serial_rb_out, serial_buffer_out, SERIAL_BUFSIZE_OUT);
  #elif defined (SERIAL_ENABLE_TX_INT_MESSAGE)
  message_init(&serial_message_out, serial_buffer_out, SERIAL_BUFSIZE_OUT);
  #endif
  
  #ifdef SERIAL_ENABLE_RX_INT
  set(SERIAL_RX_INT_CONTROL, SERIAL_RX_INT);
  #endif
  
  #ifdef SERIAL_ENABLE_RX
  set(SERIAL_RXEN_CONTROL, SERIAL_RXEN);
  #endif

  #ifdef SERIAL_ENABLE_TX
  set(SERIAL_TXEN_CONTROL, SERIAL_TXEN);
  #endif
}

void serial_change_baud(uint16_t baud)
{
	cli();
	SERIAL_BAUDRATE_REGISTER_H = (uint8_t)(baud >> 8);
	SERIAL_BAUDRATE_REGISTER_L = (uint8_t)(baud);
	sei();
}

#endif /* AVRSNIPPETS_USE_SERIAL */
