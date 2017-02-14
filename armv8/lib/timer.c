#include <common.h>
#define TIMER0_BASE 0xf8008000

#define TIMER_LOAD 		(TIMER0_BASE + 0x00)
#define TIMER_VALUE 	(TIMER0_BASE + 0x04)
#define TIMER_CTRL 		(TIMER0_BASE + 0x08)
#define TIMER_INTCLR 	(TIMER0_BASE + 0x0c)
#define TIMER_RIS		(TIMER0_BASE + 0x10)
#define TIMER_MIS 		(TIMER0_BASE + 0x14)
#define TIMER_BGLOAD 	(TIMER0_BASE + 0x18)

#define TIMER0_IRQ   46

void timer_handle(void)
{
	write32(TIMER_INTCLR, 1);
}

int get_timer_value(void)
{
	unsigned int value = 0;

	value = read32(TIMER_LOAD);
	printstr("LOAD :\t");
	print32hex(value);

	value = read32(TIMER_VALUE);
	printstr("TIMER0 :\t");
	print32hex(value);

	value = read32(TIMER_CTRL);
	printstr("CTRL :\t");
	print32hex(value);

	value = read32(TIMER_RIS);
	printstr("TIMER_RIS :\t");
	print32hex(value);

	value = read32(TIMER_MIS);
	printstr("TIMER_MIS :\t");
	print32hex(value);
	return 0;
}

void timer_init(void)
{
	write32(TIMER_LOAD, 320000 / 1000);

#if 0
	write32(TIMER_INTCLR, 0);
#endif

	write32(TIMER_CTRL, 0xf2);
	gic_enable_irq(TIMER0_IRQ, 1);
}

