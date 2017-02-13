#include <config.h>
#include <macro.h>
.global drop_to_el2

drop_to_el2:
	mov	x0, #0x5b1	/* Non-secure EL0/EL1 | HVC | 64bit EL2 */
	msr	scr_el3, x0

	mov	x0, sp
	msr	sp_el2, x0	/* Migrate SP */

	mov x0, #0x3c9
	msr spsr_el3, x0

#if 1 /* jump to defined */
	adr x1, el2_entry
	msr elr_el3, x1
#else  /* return back */
	mov elr_el3, lr
#endif
	eret

el2_entry:
	b switch_to_el2

.global drop_to_el1

drop_to_el1:
	mov x0, #0xc0000000
	msr	hcr_el2, x0

	mov	x0, sp
	msr	sp_el1, x0	/* Migrate SP */

	mov x0, #0x3c5
	msr spsr_el2, x0

#if 1 /* jump to defined */
	adr x1, el1_entry
	msr elr_el2, x1
#else  /* return back */
	mov elr_el2, lr
#endif
	eret

el1_entry:
	b switch_to_el1

