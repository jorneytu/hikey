OUTPUT_FORMAT("elf64-littleaarch64", "elf64-littleaarch64", "elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
ENTRY(start)
SECTIONS
{
	. = 0x37000000;

	. = ALIGN(8);
	.text . : {
		*start.o (.text)
		* (.text*)
		* (.rodata*)
	}

	.data . : {
		* (.data*)
	}

	. = ALIGN(0x1000);
	.pagetable (NOLOAD) : {
		__pagetable_level2 = .;
		. = __pagetable_level2 + (4096 * 3);
		__pagetable_level1 = .;
		. = __pagetable_level1 + (8 * 4);
		__pagetable_end = .;
	}

	.stack : {

		. = ALIGN(8);
		__bss_start = .;
		* (.bss)
		__bss_end = .;

		. = ALIGN(32);
		__stack_start = .;
		. = __stack_start + 0x2000;
		. = ALIGN(32);
		__stack_end = .;
	}
}
