/*
 * Copyright (c) 2014 Linaro Ltd.
 */

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
	/*
	* . = 0xf9809000;
	*/
	. = 0x07000000;
	LLOADER_START = .;

	.text :
	{
		start.o(.text)
		debug.o(.text)
		*(.rodata)
	}

	.data ALIGN(4):
	{
		*(.data)
	}

	. = ALIGN(4);

	.bss ALIGN(4):
	{
		*(.bss)
	}


	LLOADER_BL1_BIN = 0x37000000;

	. = ALIGN(0x40);

	.text : {
		lloader_aarch64_start = .;
		aarch64.o (.text)
		. = ALIGN(4);
		lloader_aarch64_end = .;
	}
}
