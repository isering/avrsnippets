/*
 * esp8266.c
 *
 * Created: 02.12.2014 21:51:49
 *  Author: Julian
 */ 

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef AVRSNIPPETS_USE_ESP8266

#include <avr/pgmspace.h>
#include <stddef.h>

#include "avrsnippets/fps_handle/fps_handle.h"
#include "avrsnippets/message/message.h"
#include "avrsnippets/serial/serial.h"
#include "esp8266.h"

#ifdef ESP8266_STATES_HEADER
#include ESP8266_STATES_HEADER
#endif

const char state_3_command[] PROGMEM = "AT+CIPMUX=1\r\n";
const char state_4_command[] PROGMEM = "AT+CIPSERVER=1,8888\r\n";

const esp8266_state_t state_3 PROGMEM = {state_3_command, state_ok, esp8266_wait_response, FRAMES_MS(5000UL), ESP8266_STATE_USER+1, ESP8266_STATE_RESET, ESP8266_STATE_RESET};
const esp8266_state_t state_4 PROGMEM = {state_4_command, state_ok, esp8266_wait_response, FRAMES_MS(5000UL), ESP8266_STATE_USER+2, ESP8266_STATE_RESET, ESP8266_STATE_RESET};

#define ESP8266_USER_STATES ,\
&state_3,\
&state_4

#ifndef ESP8266_USER_STATES
#define ESP8266_USER_STATES
#endif

#define ESP8266_AP "pf5gd8br"
#define ESP8266_PW "h5bxpt79vqu331vc91fsgksa"

static uint8_t state;
static uint16_t wait;

const char state_ok[] PROGMEM = "OK";
const char state_ready[] PROGMEM = "ready";

const char state_0_command[] PROGMEM = "AT+CWMODE=1\r\n";
const char state_1_command[] PROGMEM = "AT+RST\r\n";
const char state_2_command[] PROGMEM = "AT+CWJAP=\""  ESP8266_AP  "\",\""  ESP8266_PW  "\"\r\n";

const esp8266_state_t state_0 PROGMEM = {state_0_command, state_ok, esp8266_wait_response, FRAMES_MS(5000UL), 1, 0, 0};
const esp8266_state_t state_1 PROGMEM = {state_1_command, state_ready, esp8266_wait_response, FRAMES_MS(5000UL), 2, 0, 0};
const esp8266_state_t state_2 PROGMEM = {state_2_command, state_ok, esp8266_wait_response, FRAMES_MS(20000UL), 3, 0, 0};
	
const esp8266_state_t* const states[] PROGMEM =
{
	&state_0,
	&state_1,
	&state_2
	ESP8266_USER_STATES
};

void esp8266_init()
{
	serial_init();
	esp8266_change_state(0);
}

void esp8266_change_state(uint8_t state_next)
{
	esp8266_state_t* cur_state = (esp8266_state_t*)pgm_read_word(&states[state_next]);
	const PGM_P command = (PGM_P)pgm_read_word(&cur_state->command);
	if (command != NULL) serial_send_str(command);
	state = state_next;
	wait = pgm_read_word(&cur_state->timeout_ms);
}

uint8_t esp8266_wait_response(const message_t* message, PGM_P message_ok)
{
	if (message_equal(message, message_ok)) return ESP8266_OK;
	return ESP8266_PENDING;
}

void esp8266_handle()
{
	if (serial_message_in_ptr->status == MESSAGE_COMPLETE) {
		esp8266_state_t* cur_state = (esp8266_state_t*)pgm_read_word(&states[state]);
		const uint8_t rval = ((handle_t)pgm_read_word(&cur_state->handle))(serial_message_in_ptr, (PGM_P)pgm_read_word(&cur_state->message_ok));
		if (rval == ESP8266_OK) {
			esp8266_change_state(pgm_read_byte(&cur_state->next_ok));
			return;
		} else if (rval == ESP8266_FAIL) {
			esp8266_change_state((pgm_read_byte(&cur_state->next_fail)));
			return;
		}
	}
	
	if (wait==0) {
		esp8266_state_t* cur_state = (esp8266_state_t*)pgm_read_word(&states[state]);
		esp8266_change_state(pgm_read_byte(&cur_state->next_timeout));
	} else --wait;
}

#endif /* AVRSNIPPETS_USE_ESP8266 */