#include <config.h>
#include <common.h>
void _do_bad_sync_handle(void)
{
	printstr("_do_bad_sync_handle\n");
}

void _do_bad_irq_handle(void)
{
	printstr("_do_bad_irq_handle\n");
}

void _do_bad_fiq_handle(void)
{
	printstr("_do_bad_fiq_handle\n");
}

void _do_bad_error_handle(void)
{
	printstr("_do_bad_error_handle\n");
}

void do_sync_handle(void)
{
	printstr("do_sync_handle\n");
	get_currentel();
	while(1);
}

void do_irq_handle(void)
{
	gic_handle_irq();
}

void do_fiq_handle(void)
{
	printstr("do_fiq_handle\n");
}

void do_error_handle(void)
{
	printstr("do_error_handle\n");
}

void l64_sync_handle(void)
{
	printstr("l64_sync_handle\n");
}

void l64_irq_handle(void)
{
	printstr("l64_irq_handle\n");
}
void l64_fiq_handle(void)
{
	printstr("l64_fiq_handle\n");
}
void l64_error_handle(void)
{
	printstr("l64_error_handle\n");
}

