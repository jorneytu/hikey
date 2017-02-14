#include <config.h>
#include <common.h>

#define TIMER0_BASE 0xf8008020

#define TIMER_LOAD 		(TIMER0_BASE + 0x00)
#define TIMER_VALUE 	(TIMER0_BASE + 0x04)
#define TIMER_CTRL 		(TIMER0_BASE + 0x08)
#define TIMER_INTCLR 	(TIMER0_BASE + 0x0c)
#define TIMER_RIS		(TIMER0_BASE + 0x10)
#define TIMER_MIS 		(TIMER0_BASE + 0x14)
#define TIMER_BGLOAD 	(TIMER0_BASE + 0x18)

//#define BUS_CLOCK	32000
#define BUS_CLOCK	19200000

static char welcome_str[] = "Welcome Hikey Board ...\n";

static void timer_setup(void)
{
	
	write32(TIMER_LOAD, BUS_CLOCK);
	write32(TIMER_CTRL, 0xc2);
}

extern void Writer(void *, unsigned long, int);
extern void Reader(void *, unsigned long, int);

#if 1
#define BLOCK  5
       static unsigned long sz[] = { 
               0x200, /* 16K */
               0x4000, /* 16K */
               0x8000, /* 32K L1 cache */
               0x80000, /* 512K L2 cache */
               0x800000 /* 8M NOR memory */
       };
#else
#define BLOCK  36
       static unsigned long sz[] = { 
               256,
               512,
               768,
               1024,
               2048,
               3072,
               4096,
               6144,
               8192,   // Some processors' L1 data caches are only 8kB.
               12288,
               16384,
               20480,
               24576,
               28672,
               32768,  // Common L1 data cache size.
               40960,
               49152,
               65536,
               131072, // Old L2 cache size.
               192 * 1024,
               256 * 1024,     // Old L2 cache size.
               384 * 1024,     // Old L2 cache size.

			   512 * 1024,	// Old L2 cache size.
			   768 * 1024,
			   1 << 20,	// 1 MB = common L2 cache size.
			   (1024 + 256) * 1024,	// 1.25
			   (1024 + 512) * 1024,	// 1.5
			   (1024 + 768) * 1024,	// 1.75
			   1 << 21,	// 2 MB = common L2 cache size.
			   (2048 + 256) * 1024,	// 2.25
			   (2048 + 512) * 1024,	// 2.5
			   (2048 + 768) * 1024,	// 2.75
			   3072 * 1024,	// 3 MB = common L2 cache sized.
			   1 << 22,	// 4 MB
			   5242880,	// 5 megs
			   6291456,	// 6 megs (std L2 cache size)

       };
#endif

static void bandwidth_route(int rw)
{
	unsigned int t;
	int i, loop, L;

	if (rw)
		printstr("Writer\n\n");
	else
		printstr("Reader\n\n");

	for (i = 0; i < BLOCK; i++) 
	{
		L = 0;
		write32(TIMER_LOAD, BUS_CLOCK * 10);
		loop  = (1 << 26) / sz[i];

		do {

			if (rw)
				Writer((void *)0x000000, sz[i], loop);
			else
				Reader((void *)0x000000, sz[i], loop);
			L += loop;
			t =  (BUS_CLOCK * 10 - read32(TIMER_VALUE));

		} while(t < BUS_CLOCK);

		if (sz[i] == 0x8000)
			printstr("*\t");
		else if (sz[i] == 0x80000)
			printstr("*\t");
		else
			printstr(" \t");

		printstr("chunk size:\t");
		print32hex(sz[i]);
		printstr("\tloop: ");
		printdec(L);
		printstr("\ttime diff: ");
		printdec(t);
		printstr("\tspeed: ");
		printdec((sz[i] * L * BUS_CLOCK)/ t / 0x100000);
		printstr("MB/s\n");
	}

}
static int bandwidth(void)
{
	bandwidth_route(0);
	bandwidth_route(1);
	return 0;
}
int main(void)
{
	uart_init();
	printstr(welcome_str);

	bootstrap_gic_init();
	timer_init();
	interrupt_enable();

	timer_setup();
	bandwidth();

	for(;;);

	return 0;
}
