#ifndef __HIKEY_COMMON__
#define __HIKEY_COMMON__
#include <misc.h>
#include <mmu.h>
#include <debug.h>

#define u64 unsigned long long

#define write32(addr , v)    (*(volatile int *)((u64)addr) = (v))
#define read32(addr)         (*(volatile int *)((u64)addr))

#define write8(addr , v)    (*(volatile char *)((u64)addr) = (v))
#define read8(addr)         (*(volatile char *)((u64)addr))

int bootstrap_gic_init(void);
void gic_send_sgi(int cpumask, int irq);
int interrupt_enable(void);
int interrupt_disable(void);

void gic_handle_irq(void);
void gic_enable_irq(int irq, int type);
void timer_init(void);
int get_timer_value(void);
void watchdog_init(void);
void watchdog_status(void);
void timer_handle(void);
#endif
