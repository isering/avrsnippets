/*
 * message.c
 *
 * Created: 02.12.2014 22:28:01
 *  Author: Julian
 */ 

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef AVRSNIPPETS_USE_MESSAGE

#include "message.h"

void message_init(message_t* message, volatile uint8_t* buffer, uint8_t size)
{
	message->data = buffer;
	message->read = 0;
	message->write = 0;
	message->size = size;
	message->status = MESSAGE_READY;
}

#endif /* AVRSNIPPETS_USE_MESSAGE */