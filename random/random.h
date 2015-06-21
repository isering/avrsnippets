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
 * @file   random.h
 * @author Julian Iseringhausen <julian.iseringhausen@gmail.com>
 * @date   Wed Oct 24 23:10:35 2012
 * 
 * @brief  Lightweight random number generators.
 * 
 */

#ifndef AVR_SNIPPETS_RANDOM_H_
#define AVR_SNIPPETS_RANDOM_H_

#include <stdint.h>

extern uint32_t random_val;

extern void random_init();
extern uint32_t random_xorshift32();

extern uint8_t random_variate(uint8_t base, uint8_t bottom, uint8_t top, uint8_t var);

static inline uint8_t random_shift_8(uint8_t num_bits) __attribute__((always_inline));
static inline uint8_t random_shift_8(uint8_t num_bits)
{
  uint8_t tmp;
  asm (
  "loop%=: "
    "lsl %A[random_val]"         "\n\t"
    "rol %B[random_val]"         "\n\t"
    "rol %C[random_val]"         "\n\t"
    "rol %D[random_val]"         "\n\t"
    "mov %[tmp], %D[random_val]" "\n\t"
    "andi %[tmp], %[MASK]"       "\n\t"
    "breq no_xor%="              "\n\t" // Both bits cleared
    "cpi %[tmp], %[MASK]"        "\n\t"
    "breq no_xor%="              "\n\t" // Both bits set
    "inc %A[random_val]"         "\n\t" // LSB <- XOR
  "no_xor%=: "
    "dec %[num_bits]"            "\n\t"
    "brne loop%="
  : [random_val] "+r" (random_val),
    [num_bits]   "+r" (num_bits),
    [tmp]        "=d" (tmp)
  : [MASK]       "M"  ((1<<4) | (1<<7))
  );
  return (uint8_t)random_val;
}

static inline uint16_t random_shift_16(uint8_t num_bits) __attribute__((always_inline));
static inline uint16_t random_shift_16(uint8_t num_bits)
{
  uint8_t tmp;
  asm (
  "loop%=: "
    "lsl %A[random_val]"         "\n\t"
    "rol %B[random_val]"         "\n\t"
    "rol %C[random_val]"         "\n\t"
    "rol %D[random_val]"         "\n\t"
    "mov %[tmp], %D[random_val]" "\n\t"
    "andi %[tmp], %[MASK]"       "\n\t"
    "breq no_xor%="              "\n\t" // Both bits cleared
    "cpi %[tmp], %[MASK]"        "\n\t"
    "breq no_xor%="              "\n\t" // Both bits set
    "inc %A[random_val]"         "\n\t" // LSB <- XOR
  "no_xor%=: "
    "dec %[num_bits]"            "\n\t"
    "brne loop%="
  : [random_val] "+r" (random_val),
    [num_bits]   "+r" (num_bits),
    [tmp]        "=d" (tmp)
  : [MASK]       "M"  ((1<<4) | (1<<7))
  );
  return (uint16_t)random_val;
}

#endif /* AVR_SNIPPETS_RANDOM_H_ */
