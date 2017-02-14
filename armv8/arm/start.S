.text

.global start
start:
	b reset

	.align 3
vector:
.quad(__vectors)

_bss_start:
.quad(__bss_start)

_bss_end:
.quad(__bss_end)

_stack_end:
.quad(__stack_end)

reset:

	ldr x0, vector
	msr	vbar_el3, x0

	/* Exception Routing Setup */
	mrs	x0, scr_el3
	bic x0, x0, #1
	orr	x0, x0, #0xe
	msr	scr_el3, x0

	bl mmu_init

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
