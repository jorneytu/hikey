#include <config.h>
#include <macro.h>

.global start
start:
	b reset

	.align 3
vector:
.quad(_vectors)

_bss_start:
.quad(__bss_start)

_bss_end:
.quad(__bss_end)

_stack_end:
.quad(__stack_end)

.global pgtable_start
pgtable_start:
.quad(__pgtable_start)

/*
 * Initial state:
 * 		EL3 mode
 *  	Little Endian
 *  	MMU Disabled
 *  	I/D Cache Disabled
 */

reset:

	bl armv8_el_setup



#ifdef CORTEXA
	mrs x0, S3_1_c15_c2_1
	orr x0, x0, #(1 << 6)   // Set the SMPEN
	msr S3_1_c15_c2_1, x0
#endif

	bl armv8_core_switch


arm_primary:

	//
	// Enable GICD
	//
	//bl GICEnable

	//
	// Setup TLB Enable MMU & Caches
	//
	//bl mem_setup


stack_setup:
	ldr x0, _stack_end
	bic x0, x0, #0xf
	mov sp, x0

clear_bss:
	ldr x0, _bss_start
	ldr x1, _bss_end
clear_loop:
	str xzr, [x0]
	add x0, x0, #0x8
	cmp x0, x1
	b.lo clear_loop

	b main

//////////////////////////////////////////////////////////////////////

.global armv8_el_setup
armv8_el_setup:
	ldr x0, vector

	armv8_switch_el x1, el3, el2, el1
el3:
	msr	vbar_el3, x0

	mrs	x0, scr_el3
	bic x0, x0, #1
//	orr	x0, x0, #0xf
	orr	x0, x0, #0xe
	msr	scr_el3, x0

//	ret
el2:
	ldr x0, vector
	msr vbar_el2, x0

//	ret
el1:
	ldr x0, vector
	msr	vbar_el1, x0

	ret

.global armv8_core_switch
armv8_core_switch:

	mov x28, x30
	mrs x0, mpidr_el1
	lsr x1, x0, #32
	lsr x2, x0, #24

	and x1, x1, #0xff
	cbz x1, master

	and x2, x2, #0x1
	cbz x1, master

slave:

#ifdef CONFIG_ARMV8_EL1
	bl drop_to_el1
#endif
	//
	// TODO  SP Initial ...
	//
	wfi

master:
	//
	// GIC Setup setep 1
	//
	bl GICSetup
	mov x30, x28
	ret

mem_setup:
	mov x28, x30

	bl mmu_enable
	bl get_sctlr
	ldr x1, =(SC_ICACHE_ENABLE|SC_DCACHE_ENABLE)
	orr x0, x0, x1
	bl set_sctlr

	mov x30, x28
	ret


/////////////////////////////////////////////////////////////

