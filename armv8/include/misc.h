#ifndef __HIKEY_MISC
#define __HIKEY_MISC
#include <board.h>

void printch(char );
void uart_init(void);
void printstr(char *s);
void print32hex(int x);
void print64hex(long long x);

void gpio_set(int);
void gpio_clear(int);

int mdelay(int);

void set_sctlr(unsigned int);
unsigned int get_sctlr(void);

void icache_enable(void);
void icache_disable(void);
void invalidate_icache_all(void);
void _invalidate_icache_all(void);
void _invalidate_dcache_all(void);
void _flush_dcache_all(void);
void _invalidate_tlb_all(void);

void dcache_enable(void);
void dcache_disable(void);
void invalidate_dcache_all(void);
void flush_dcache_all(void);

void enable_caches(void);

#endif
