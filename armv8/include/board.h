#ifndef __BOARD_H__
#define __BOARD_H__

#define DRAM_START    0x0
#define DRAM_SIZE     0x40000000

#define HIKEY_GPIO4_BASE  0xf7020000
#define GPIO_DATA_0       0x04
#define GPIO_DATA_1       0x08
#define GPIO_DATA_2       0x10
#define GPIO_DATA_3       0x20
#define GPIO_DIR          0x400


#define UART3_RXD  0xf7010188
#define UART3_TXD  0xf701018c
#define PERI_CTRL  				0xf7030000
#define SC_PERIPH_RSTDIS3		0x334
#define SC_PERIPH_CLKEN3		0x230

#define DEBUG_UART_PHYS         0xf8015000
#define UART01x_DR				0x00
#define UART01x_FR				0x18
#define UART01x_FR_TXFF			0x020
#define UART01x_FR_BUSY			0x008

#endif
