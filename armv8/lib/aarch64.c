#include <config.h>
#include <common.h>
#define VISIBLE 1

int get_currentel(void)
{
	unsigned int value = 0;

	asm volatile("mrs  %0, CurrentEL"
			:"=r"(value)
			:
			: "memory");

#if VISIBLE
	printstr("CurrentEL:\t");
	print32hex(value);
#endif

	return value;
}

int interrupt_enable(void)
{
	unsigned int value = 0;
	unsigned int tmp = 0;


#if 1
	asm volatile( "mrs %0, DAIF"
			: "=r"(value)
			:
			: "memory");

	tmp = (value & 0xfe3f);
#endif
	mdelay(100);

	asm volatile( "msr DAIF, %0"
			: "=r"(tmp)
			:
			: "memory");

	return 0;

}
int interrupt_disable(void)
{
	unsigned int value = 0;
	unsigned int tmp = 0;

	asm volatile( "mrs %0, DAIF"
			: "=r"(value)
			:
			: "memory");

	tmp = value | 0x1c0;

	asm volatile( "msr DAIF, %0"
			: "=r"(tmp)
			:
			: "memory");

	return 0;

}
