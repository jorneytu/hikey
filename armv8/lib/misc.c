#include <config.h>
#include <misc.h>
#include <common.h>


static char hex2ch[] = {"0123456789abcdef"};

void uart_init()
{
	write32(UART3_RXD, 1);
	write32(UART3_TXD, 1);

	write32(PERI_CTRL + SC_PERIPH_RSTDIS3, 1 << 7);

	write32(PERI_CTRL + SC_PERIPH_CLKEN3, 1 << 7);
}

void waituart(void)
{
	unsigned int value = 0;
	do{
		value = read32(DEBUG_UART_PHYS + UART01x_FR);
	}while( (value & UART01x_FR_TXFF) == UART01x_FR_TXFF);
}

void busyuart(void)
{
	unsigned int value = 0;
	do{
		value = read32(DEBUG_UART_PHYS + UART01x_FR);
	}while((value & UART01x_FR_BUSY) == UART01x_FR_BUSY);
}

void senduart(char c)
{
	write8(DEBUG_UART_PHYS + UART01x_DR, c);
}

void uart_putchar(char c)
{
	waituart();
	senduart(c);
	busyuart();
}
void printch(char c)
{
	if(c == '\n'){
		uart_putchar('\r');
	}
	uart_putchar(c);
}

void printstr(char *s)
{
	char *p = s;
	while(*p != '\0'){
		printch(*p++);
	}
}
void print32hex(int n)
{
	int i, m;
	char a[10] = {0};
	a[8] = '\n';
	a[9] = '\0';
	for (i = 0; i < 8; i++){
		m = n & 0xf;
		a[7 - i] = hex2ch[m];
		n = n >> 4;
	}
	printstr("0x");
	printstr(a);
}

void print64hex(long long n)
{
	int i, m;
	char a[18] = {0};
	a[16] = '\n';
	a[17] = '\0';
	for (i = 0; i < 16; i++){
		m = n & 0xf;
		a[15 - i] = hex2ch[m];
		n = n >> 4;
	}
	printstr("0x");
	printstr(a);
}

static long long get_armv8_timer(void)
{
	long long cnt = 0;
	asm volatile("mrs %0 , CNTPCT_EL0"
			: "=r" (cnt)
			:
			: "memory");
	return cnt;
}
int mdelay(int msec)
{
	long long old, new;

	old = new = get_armv8_timer();

	do {
		new = get_armv8_timer();
	}while(new - old < msec * 1000);

	return 0;
}


//#define HIKEY_GPIO4_BASE  0xf7020000
//#define GPIO_DATA_0       0x04
//#define GPIO_DATA_1       0x08
//#define GPIO_DATA_2       0x10
//#define GPIO_DATA_3       0x20
//#define GPIO_DIR          0x400
//
//
//.global led_flush
//led_flush:
//
//	mov x29, x30
//1:
//	mov x1, #1
//	mov x0, #GPIO_DATA_0
//	bl led_show
//	mov x0, #GPIO_DATA_1
//	bl led_show
//	mov x0, #GPIO_DATA_2
//	bl led_show
//	mov x0, #GPIO_DATA_3
//	bl led_show
//	cmp x28, #1
//	b.eq 1b
//
//	mov x30, x29
//	ret
//
//
//.global led_show
//
//led_show:
//	ldr x3, =HIKEY_GPIO4_BASE
//	add x4, x3, #GPIO_DIR
//	mov w2, #0xff
//	str w2, [x4]
//
//	mov w2, #0xff
//	add x4, x3, x0
//	str w2, [x4]
//	mov w4, #0x100000
//1000:
//	sub w4, w4, #1
//	cmp w4, #0
//	bne 1000b
//
//	mov w2, #0x0
//	add x4, x3,x0
//	cmp x1, #1
//	b.ne out
//	str w2, [x4]
//out:
//	ret


void gpio_dir_set(int gpio, int dir)
{
	unsigned int value;

	if (dir){
		value = read32(HIKEY_GPIO4_BASE + GPIO_DIR);
		value |= (0x01 << gpio);
		write32(HIKEY_GPIO4_BASE + GPIO_DIR , value);
	}else{
		value = read32(HIKEY_GPIO4_BASE + GPIO_DIR);
		value &= ~(0x01 << gpio);
		write32(HIKEY_GPIO4_BASE + GPIO_DIR , value);
	}
}

void gpio_set(int gpio)
{

	gpio_dir_set(gpio, 1);

	write32(HIKEY_GPIO4_BASE + (0x04 << gpio), 0xff);
}

void gpio_clear(int gpio)
{

	gpio_dir_set(gpio, 1);

	write32(HIKEY_GPIO4_BASE + (0x04 << gpio), 0);
}
