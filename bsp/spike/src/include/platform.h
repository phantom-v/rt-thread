// See LICENSE for license details.

#ifndef _SIFIVE_PLATFORM_H
#define _SIFIVE_PLATFORM_H

// Some things missing from the official encoding.h
#if __riscv_xlen == 32
#define MCAUSE_INT         (1U << 31)
#else
#define MCAUSE_INT         (1ULL << 63)
#endif

#define MCAUSE_CAUSE       (~MCAUSE_INT)

#include "const.h"
#include "clint.h"
#include "plic.h"
#include "uart.h"

/****************************************************************************
 * Platform definitions
 *****************************************************************************/

// Memory map
#define MASKROM_MEM_ADDR _AC(0x00001000,UL)
#define TRAPVEC_TABLE_CTRL_ADDR _AC(0x00001010,UL)
#define CLINT_CTRL_ADDR _AC(0x02000000,UL)
#define PLIC_CTRL_ADDR _AC(0x0C000000,UL)
#define UART0_CTRL_ADDR _AC(0x10000000,UL)
#define MEM_CTRL_ADDR _AC(0x80000000,UL)


// Interrupt numbers
#define INT_RESERVED 0
#define INT_WDOGCMP 1
#define INT_RTCCMP 2
#define INT_UART0_BASE 3
#define INT_UART1_BASE 4
#define INT_SPI0_BASE 5
#define INT_SPI1_BASE 6
#define INT_SPI2_BASE 7
#define INT_GPIO_BASE 8
#define INT_PWM0_BASE 40
#define INT_PWM1_BASE 44
#define INT_PWM2_BASE 48

// Helper functions
#define _REG8(p, i) (*(volatile uint8_t *) ((p) + (i)))
#define _REG32(p, i) (*(volatile uint32_t *) ((p) + (i)))
#define _REG32P(p, i) ((volatile uint32_t *) ((p) + (i)))
#define CLINT_REG(offset) _REG32(CLINT_CTRL_ADDR, offset)
#define PLIC_REG(offset) _REG32(PLIC_CTRL_ADDR, offset)
#define UART0_REG(offset) _REG8(UART0_CTRL_ADDR, offset)


// Misc

#include <stdint.h>

#define NUM_GPIO 32

#define PLIC_NUM_INTERRUPTS 52
#define PLIC_NUM_PRIORITIES 7

#define RTC_FREQ 32768

unsigned long get_cpu_freq(void);
unsigned long get_timer_freq(void);
uint64_t get_timer_value(void);

#endif /* _SIFIVE_PLATFORM_H */
