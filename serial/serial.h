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
 * serial.h
 *
 * Created: 21.10.2012 12:08:00
 *  Author: Julian Iseringhausen <julian.iseringhausen@gmail.com>
 */ 


#ifndef SERIAL_H_
#define SERIAL_H_

/*
 * DEFINES
 * 
 * SERIAL_ENABLE_RX                 Enable UART receive
 * SERIAL_ENABLE_TX                 Enable UART transmit
 * SERIAL_ENABLE_RX_INT             Enable UART receive interrupt
 * SERIAL_ENABLE_TX_INT             Enable UART transmit interrupt
 * SERIAL_ENABLE_RX_INT_DEFAULT     Enable UART receive through ringbuffer
 * SERIAL_ENABLE_TX_INT_DEFAULT     Enable more UART transmit functions
 * SERIAL_ENABLE_DOUBLE_SPEED       Set prescaler to 8 instead of 16
 * SERIAL_BAUDRATE                  Set UART baudrate
 * SERIAL_BUFSIZE_IN				Set size of input buffer
 * SERIAL_BUFSIZE_OUT
 *
 */

#ifdef AVRSNIPPETS_USE_SERIAL

#include <stdint.h>

#include <avr/io.h>

#include "avrsnippets/helper.h"

#include "avrsnippets/message/message.h"
#include "avrsnippets/ringbuffer/ringbuffer.h"

#if defined(SERIAL_ENABLE_RX_INT_RINGBUFFER) && defined(SERIAL_ENABLE_RX_INT_MESSAGE)
#error AVRSNIPPETS SERIAL: Multiple RX interrupt routines chosen
#endif

#if defined(SERIAL_ENABLE_TX_INT_RINGBUFFER) && defined(SERIAL_ENABLE_TX_INT_MESSAGE)
#error AVRSNIPPETS SERIAL: Multiple TX interrupt routines chosen
#endif

#ifdef SERIAL_ENABLE_RX_INT_RINGBUFFER

#ifndef SERIAL_ENABLE_RX_INT
#define SERIAL_ENABLE_RX_INT
#endif

#ifndef SERIAL_BUFSIZE_IN
#define SERIAL_BUFSIZE_IN 32
#endif

extern ringbuffer_t serial_rb_in;

#endif /* SERIAL_ENABLE_RX_INT_RINGBUFFER */

#ifdef SERIAL_ENABLE_TX_INT_RINGBUFFER

#ifndef SERIAL_ENABLE_TX_INT
#define SERIAL_ENABLE_TX_INT
#endif

#ifndef SERIAL_BUFSIZE_OUT
#define SERIAL_BUFSIZE_OUT 32
#endif

extern ringbuffer_t serial_rb_out;

#endif /* SERIAL_ENABLE_TX_INT_RINGBUFFER */

#ifdef SERIAL_ENABLE_RX_INT_MESSAGE

#ifndef SERIAL_ENABLE_RX_INT
#define SERIAL_ENABLE_RX_INT
#endif

#ifndef SERIAL_BUFSIZE_IN
#define SERIAL_BUFSIZE_IN 32
#endif

extern message_t serial_message_in;

#endif /* SERIAL_ENABLE_RX_INT_MESSAGE */

#ifdef SERIAL_ENABLE_RX_INT_MESSAGE_BUFFERED

#ifndef SERIAL_ENABLE_RX_INT
#define SERIAL_ENABLE_RX_INT
#endif

#ifndef SERIAL_BUFSIZE_IN
#define SERIAL_BUFSIZE_IN 255
#endif

extern message_t* volatile serial_message_in_ptr;

extern message_t serial_message_in1;
extern message_t serial_message_in2;

#endif /* SERIAL_ENABLE_RX_INT_MESSAGE_BUFFERED */

#ifdef SERIAL_ENABLE_TX_INT_MESSAGE

#ifndef SERIAL_ENABLE_TX_INT
#define SERIAL_ENABLE_TX_INT
#endif

#ifndef SERIAL_BUFSIZE_OUT
#define SERIAL_BUFSIZE_OUT 32
#endif

extern message_t serial_message_out;

#endif /* SERIAL_ENABLE_TX_INT_MESSAGE */

#ifdef SERIAL_ENABLE_RX_INT
#define SERIAL_ENABLE_RX
#endif

#ifdef SERIAL_ENABLE_TX_INT
#define SERIAL_ENABLE_TX
#endif

#ifndef SERIAL_SET_BAUDRATE_DIRECT

#define SERIAL_UBRR_VAL ((F_CPU+SERIAL_BAUDRATE*8)/(SERIAL_BAUDRATE*16)-1)
#define SERIAL_BAUDRATE_REAL (F_CPU/(16*(SERIAL_UBRR_VAL+1)))
#define SERIAL_BAUDRATE_ERROR ((SERIAL_BAUDRATE_REAL*1000)/SERIAL_BAUDRATE)

#if ((SERIAL_BAUDRATE_ERROR<980) || (SERIAL_BAUDRATE_ERROR>1020))
#error UART: Systematic baudrate error too high. Please adjust F_CPU / UART_BAUDRATE
#endif

#endif /* !SERIAL_SET_BAUDRATE_DIRECT */

#if defined(__AVR_ATmega644P__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega88__) || defined(__AVR_ATmega48__) || defined (__AVR_ATmega328P__)

#define serial_set_bit_9() (set(UCSR0B, TXB80))
#define serial_clear_bit_9() (clear(UCSR0B, TXB80))
#define serial_toggle_bit_9() (toggle(UCSR0B, TXB80))
#define serial_get_bit_9() (get(UCSR0B, RXB80))

#define SERIAL_BAUDRATE_REGISTER_H UBRR0H
#define SERIAL_BAUDRATE_REGISTER_L UBRR0L

#define SERIAL_DATA_REGISTER UDR0

#define SERIAL_RXEN_CONTROL UCSR0B
#define SERIAL_RXEN RXEN0

#define SERIAL_TXEN_CONTROL UCSR0B
#define SERIAL_TXEN TXEN0

#define SERIAL_RX_INT_CONTROL UCSR0B
#define SERIAL_RX_INT RXCIE0

#define SERIAL_TX_INT_CONTROL UCSR0B
#define SERIAL_TX_INT TXCIE0

#define SERIAL_U2X_CONTROL UCSR0A
#define SERIAL_U2X U2X0

#define SERIAL_CHARSIZE_CONTROL_1 UCSR0C
#define SERIAL_CHARSIZE_CONTROL_2 UCSR0B

#define SERIAL_CHARSIZE_5BIT_1 0
#define SERIAL_CHARSIZE_5BIT_2 0

#define SERIAL_CHARSIZE_6BIT_1 _BV(UCSZ00)
#define SERIAL_CHARSIZE_6BIT_2 0

#define SERIAL_CHARSIZE_7BIT_1 _BV(UCSZ01)
#define SERIAL_CHARSIZE_7BIT_2 0

#define SERIAL_CHARSIZE_8BIT_1 (_BV(UCSZ00) | _BV(UCSZ01))
#define SERIAL_CHARSIZE_8BIT_2 0

#define SERIAL_CHARSIZE_9BIT_1 (_BV(UCSZ00) | _BV(UCSZ01))
#define SERIAL_CHARSIZE_9BIT_2 _BV(UCSZ02)

#define SERIAL_DRE_CONTROL UCSR0A
#define SERIAL_DRE UDRE0

#define SERIAL_RXC_CONTROL UCSR0A
#define SERIAL_RXC RXC0

#define SERIAL_DRE_INT_CONTROL UCSR0B
#define SERIAL_DRE_INT UDRIE0

#ifdef __AVR_ATmega644P__

#define SERIAL_RX_vect USART0_RX_vect
#define SERIAL_TX_vect USART0_TX_vect
#define SERIAL_UDRE_vect USART0_UDRE_vect

#elif defined(__AVR_ATmega168__) || defined(__AVR_ATmega88__) || defined(__AVR_ATmega48__) || defined(__AVR_ATmega328P__)

#define SERIAL_RX_vect USART_RX_vect
#define SERIAL_TX_vect USART_TX_vect
#define SERIAL_UDRE_vect USART_UDRE_vect

#endif

#elif defined __AVR_ATmega8__

#error set rest of defines

#define SERIAL_BAUDRATE_REGISTER_H UBRRH
#define SERIAL_BAUDRATE_REGISTER_L UBRRL

#define SERIAL_DATA_REGISTER UDR

#define SERIAL_RXEN_CONTROL UCSRB
#define SERIAL_RXEN RXEN

#define SERIAL_TXEN_CONTROL UCSRB
#define SERIAL_TXEN TXEN

#define SERIAL_RX_INT_CONTROL UCSRB
#define SERIAL_RX_INT RXCIE

#define SERIAL_TX_INT_CONTROL UCSRB
#define SERIAL_TX_INT TXCIE

#define SERIAL_CHARSIZE_CONTROL UCSRC
#define SERIAL_CHARSIZE_8BIT ((1<<UCSZ0)|(1<<UCSZ1))

#define SERIAL_DRE_CONTROL UCSRA
#define SERIAL_DRE UDRE

#define SERIAL_RXC_CONTROL UCSRA
#define SERIAL_RXC RXC

#define SERIAL_DRE_INT_CONTROL UCSRB
#define SERIAL_DRE_INT UDRIE

#define SERIAL_RX_vect USART_RXC_vect
#define SERIAL_TX_vect USART_TXC_vect
#define SERIAL_UDRE_vect USART_UDRE_vect

#error Check defines

#else

#error Microcontroller not compatible to USART

#endif

#if !defined(SERIAL_CHARSIZE) || SERIAL_CHARSIZE < 5 || SERIAL_CHARSIZE > 9
#error serial: set SERIAL_CHARSIZE properly
#endif

extern void serial_init();
extern void serial_change_baud(uint16_t baud);
extern void serial_send(uint8_t data);
extern void serial_send_str(PGM_P str);
extern void serial_send_array(uint8_t* data, uint8_t size);

static inline void serial_send_str_direct(PGM_P str) __attribute__((always_inline));
static inline void serial_send_str_direct(PGM_P str)
{
	uint8_t i=0;
	uint8_t data;
	do {
		data = pgm_read_byte(&str[i++]);
		while (!get(SERIAL_DRE_CONTROL, SERIAL_DRE));
		SERIAL_DATA_REGISTER = data;
	} while (data != 0);
}

static inline void serial_send_direct(uint8_t data) __attribute__(( always_inline ));
static        void serial_send_direct(uint8_t data)
{
	while (!get(SERIAL_DRE_CONTROL, SERIAL_DRE));
	SERIAL_DATA_REGISTER = data;
}


static inline uint8_t serial_receive_wait() __attribute__(( always_inline ));
static        uint8_t serial_receive_wait()
{
	while (!get(SERIAL_RXC_CONTROL, SERIAL_RXC));
	return SERIAL_DATA_REGISTER;
}

static inline uint8_t serial_receive_nowait() __attribute__(( always_inline ));
static        uint8_t serial_receive_nowait()
{
	if (get(SERIAL_RXC_CONTROL, SERIAL_RXC))
	  return SERIAL_DATA_REGISTER;
    else
      return 0;
}

#endif /* AVRSNIPPETS_USE_SERIAL */

#endif /* SERIAL_H_ */
