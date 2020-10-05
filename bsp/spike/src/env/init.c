#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "platform.h"
#include "encoding.h"

extern int main(int argc, char** argv);
extern void trap_entry();

static unsigned long mtime_lo(void)
{
  return *(volatile unsigned long *)(CLINT_CTRL_ADDR + CLINT_MTIME);
}

#ifdef __riscv32

static uint32_t mtime_hi(void)
{
  return *(volatile uint32_t *)(CLINT_CTRL_ADDR + CLINT_MTIME + 4);
}

uint64_t get_timer_value()
{
  while (1) {
    uint32_t hi = mtime_hi();
    uint32_t lo = mtime_lo();
    if (hi == mtime_hi())
      return ((uint64_t)hi << 32) | lo;
  }
}

#else /* __riscv32 */

uint64_t get_timer_value()
{
  return mtime_lo();
}

#endif

unsigned long get_timer_freq()
{
  return 32768;
}


static unsigned long __attribute__((noinline)) measure_cpu_freq(size_t n)
{
  unsigned long start_mtime, delta_mtime;
  unsigned long mtime_freq = get_timer_freq();

  // Don't start measuruing until we see an mtime tick
  unsigned long tmp = mtime_lo();
  do {
    start_mtime = mtime_lo();
  } while (start_mtime == tmp);

  unsigned long start_mcycle = read_csr(mcycle);

  do {
    delta_mtime = mtime_lo() - start_mtime;
  } while (delta_mtime < n);

  unsigned long delta_mcycle = read_csr(mcycle) - start_mcycle;

  return (delta_mcycle / delta_mtime) * mtime_freq
         + ((delta_mcycle % delta_mtime) * mtime_freq) / delta_mtime;
}

unsigned long get_cpu_freq()
{
  static uint32_t cpu_freq;

  if (!cpu_freq) {
    // warm up I$
    measure_cpu_freq(1);
    // measure for real
    cpu_freq = measure_cpu_freq(10);
  }

  return cpu_freq;
}

static void uart_init(size_t baud_rate)
{
    uint32_t divisor = get_cpu_freq() / (16 * baud_rate);
    UART0_REG(UART_LCR) = UART_LCR_DLAB;
    UART0_REG(UART_DLL) = divisor & 0xff;
    UART0_REG(UART_DLM) = (divisor >> 8) & 0xff;

    UART0_REG(UART_LCR) = UART_LCR_8BIT | UART_LCR_PNONE;
}



#ifdef USE_PLIC
extern void handle_m_ext_interrupt();
#endif

#ifdef USE_M_TIME
extern void handle_m_time_interrupt();
#endif

uintptr_t handle_trap(uintptr_t mcause, uintptr_t epc)
{
  if (0){
#ifdef USE_PLIC
    // External Machine-Level interrupt from PLIC
  } else if ((mcause & MCAUSE_INT) && ((mcause & MCAUSE_CAUSE) == IRQ_M_EXT)) {
    handle_m_ext_interrupt();
#endif
#ifdef USE_M_TIME
    // External Machine-Level interrupt from PLIC
  } else if ((mcause & MCAUSE_INT) && ((mcause & MCAUSE_CAUSE) == IRQ_M_TIMER)){
    handle_m_time_interrupt();
#endif
  }
  else {
    rt_kprintf("Unhandled Trap.\n");
  }
  return epc;
}

void _init()
{

  #ifndef NO_INIT
  use_default_clocks();
  use_pll(0, 0, 1, 31, 1);
  uart_init(115200);

  rt_kprintf("core freq at %ld Hz\n", get_cpu_freq());

  write_csr(mtvec, &trap_entry);
  if (read_csr(misa) & (1 << ('F' - 'A'))) { // if F extension is present
    write_csr(mstatus, MSTATUS_FS); // allow FPU instructions without trapping
    write_csr(fcsr, 0); // initialize rounding mode, undefined at reset
  }
  #endif

}

void _fini()
{
}
