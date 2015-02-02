/*
 * esp8266.h
 *
 * Created: 02.12.2014 21:47:52
 *  Author: Julian
 */ 


#ifndef ESP8266_H_
#define ESP8266_H_

#include "avrsnippets/message/message.h"

#define ESP8266_OK 0
#define ESP8266_PENDING 1
#define ESP8266_FAIL 2

#define ESP8266_STATE_RESET 0
#define ESP8266_STATE_USER 3

void esp8266_init();
void esp8266_handle();

void esp8266_change_state(uint8_t state);

uint8_t esp8266_wait_response(const message_t* message, PGM_P message_ok);

typedef uint8_t (*handle_t)(const message_t*, PGM_P);

typedef struct {
	PGM_P command;
	PGM_P message_ok;
	handle_t handle;
	uint16_t timeout_ms;
	uint8_t next_ok;
	uint8_t next_fail;
	uint8_t next_timeout;
} esp8266_state_t;

extern const char state_ok[] PROGMEM;
extern const char state_ready[] PROGMEM;

#endif /* ESP8266_H_ */