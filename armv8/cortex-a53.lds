OUTPUT_FORMAT("elf64-littleaarch64", "elf64-littleaarch64", "elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
ENTRY(start)
SECTIONS
{
	. = 0x37000000;

	. = ALIGN(8);
	.text :{
		arm/start.o (.text*)
	}

	. = ALIGN(8);
	.data : {
		*(.data*)
	}

	. = ALIGN(8);
	.bss (NOLOAD) : {

	__bss_start = .;

		*(.bss)

	__bss_end = .;

	. = ALIGN(8);

	__stack_start = .;
	. = __stack_start + 0x400000;
	__stack_end = .;
	}

	. = ALIGN(0x1000000);
	__pgtable_start = .;
}
