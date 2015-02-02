/*
 * helper.h
 *
 * Created: 21.10.2012 12:10:58
 *  Author: Julian
 */ 


#ifndef HELPER_H_
#define HELPER_H_

#include <stdint.h>

#define set(port, pin) ((port) |= (uint8_t)(1 << (pin)))
#define clear(port, pin) ((port) &= (uint8_t)~(1 << (pin)))
#define toggle(port, pin) ((port) ^= (uint8_t)(1 << (pin)))
#define get(port, pin) (((port) & (1 << (pin))) ? 1 : 0)
#define pulse(port, pin) \
  do { \
    set((port), (pin)); \
    clear((port), (pin)); \
  } while (0)


#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

#endif /* HELPER_H_ */