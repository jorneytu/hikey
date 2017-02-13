#include <config.h>
#include <common.h>

extern void drop_to_el2(void);
extern void drop_to_el1(void);
#define DEFAULT_MODE_EL1 1

static char welcome_str[] = "Welcome Hikey Board ...\n";

void led_display(void)
{
	gpio_set(0);
	mdelay(1000);
	gpio_clear(0);

	gpio_set(1);
	mdelay(1000);
	gpio_clear(1);

	gpio_set(2);
	mdelay(1000);
	gpio_clear(2);

	gpio_set(3);
	mdelay(1000);
	gpio_clear(3);
}

void mem_test(void)
{
	int i;
	unsigned int *s1 = (unsigned int *)0x4;
	unsigned int *s2 = (unsigned int *)0x900000;
	unsigned int *_s1 = s1;
	unsigned int *_s2 = s2;

	printstr("memory test ...\n");

	for(i = 0; i < 0x100; i++){
		*_s2++ = *_s1++;
	}

	for(i = 0; i < 0x100; i++){
		if (*s2++ != *s1++) {
			printstr("memory in trouble\n");
			break;
		}
	}
}

void debug(void)
{
	int value = 0;
	asm volatile("mrs %0, sctlr_el3"
			:"+r"(value)
			:
			: "memory");

	if ((value & SC_MMU_ENABLE) == SC_MMU_ENABLE)
		printstr("MMU enable\n");

	if ((value & SC_DCACHE_ENABLE) == SC_DCACHE_ENABLE)
		printstr("D-Cache enable\n");

	if ((value & SC_ICACHE_ENABLE) == SC_ICACHE_ENABLE)
		printstr("I-Cache enable\n");

}

int main(void)
{

	uart_init();
	printstr(welcome_str);

#if 0
	enable_caches();
#endif

	debug();
#if 0
	drop_to_el2();
#endif

	interrupt_disable();
	bootstrap_gic_init();
	interrupt_enable();
	timer_init();
	gic_send_sgi(0x1, 11);
	while (1){
		gic_send_sgi(0x1, 11);
		mem_test();
		led_display();
		//get_timer_value();
		interrupt_enable();
		//gic_debug(46);
#if 0
		asm volatile( "mrs %0, DAIF"
			: "=r"(value)
			:
			: "memory");
		print32hex(value);
#endif
	}

	return 0;
}

int switch_to_el2(void)
{
	unsigned int value = 0;
	long long _value = 0;
#if DEFAULT_MODE_EL1
	drop_to_el1();
#endif
	printstr("el2 mode ...!\n");

	asm volatile("mrs %0 , CurrentEL"
			: "=r" (value)
			:
			: "memory");
	printstr("CurrentEL:\t");
	print32hex(value);

	asm volatile("mrs %0 , SCTLR_EL2"
			: "=r" (_value)
			:
			: "memory");
	printstr("SCTLR_EL2:\t");
	print64hex(_value);
	while(1);
}

int switch_to_el1(void)
{
	unsigned int value = 0;

	printstr("el1 mode ...!\n");

	asm volatile("mrs %0 , CurrentEL"
			: "=r" (value)
			:
			: "memory");
	printstr("CurrentEL:\t");
	print32hex(value);

#if EL1_IS_SECURE
	asm volatile("mrs %0 , SCTLR_EL1"
			: "=r" (_value)
			:
			: "memory");
	printstr("SCTLR_EL1:\t");
	print64hex(_value);
#endif


	interrupt_disable();
	bootstrap_gic_init();
	interrupt_enable();


	gic_send_sgi(0x1, 1);
	while(1){
		mem_test();
		led_display();
	}
}
