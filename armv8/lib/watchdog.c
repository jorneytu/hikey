#include <common.h>
#define WATCHDOG_BASE   0xf8005000

#define  WDG_LOAD      (WATCHDOG_BASE + 0x00)
#define  WDG_VALUE     (WATCHDOG_BASE + 0x04)
#define  WDG_CTRL      (WATCHDOG_BASE + 0x08)
#define  WDG_INTCLR    (WATCHDOG_BASE + 0x0c)
#define  WDG_RIS       (WATCHDOG_BASE + 0x10)
#define  WDG_MIS       (WATCHDOG_BASE + 0x14)
#define  WDG_LOCK      (WATCHDOG_BASE + 0x18)

#define WDG_IRQ     45

void watchdog_status(void)
{
	unsigned int value = 0;

	value = read32(WDG_LOAD);
	printstr("LOAD :\t");
	print32hex(value);

	value = read32(WDG_VALUE);
	printstr("VALUE :\t");
	print32hex(value);

	value = read32(WDG_RIS);
	printstr("RIS :\t");
	print32hex(value);

	value = read32(WDG_MIS);
	printstr("MIS :\t");
	print32hex(value);

	value = read32(WDG_LOCK);
	printstr("LOCK :\t");
	print32hex(value);

}
void watchdog_init(void)
{
	write32(WDG_LOCK, 0x1acce551);
	write32(WDG_LOAD, 0x100);
	write32(WDG_CTRL, 1);
	write32(WDG_LOCK, 0x0badbeef);
	gic_enable_irq(WDG_IRQ, 1);
}
