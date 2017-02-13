#include <config.h>

.global GICSetup
GICSetup:
	mov x29, x30
	ldr x0, =0x80000
	ldr	w9, [x0, GICD_TYPER]
	and	w10, w9, #0x1f			/* ITLinesNumber */
	cbz	w10, 1f					/* No SPIs */
	add	x11, x0, (GICD_IGROUPRn + 4)
	add	x12, x0, (GICD_IGROUPMODRn + 4)
	mov	w9, #~0
0:	str	w9, [x11], #0x4
	str	wzr, [x12], #0x4		/* Config SPIs as Group1NS */
	sub	w10, w10, #0x1
	cbnz	w10, 0b

1:
	mov x30, x29
	ret

.global GICEnable
GICEnable:
	ldr x0, =0x80000
	mov	w9, #0x37			/* EnableGrp0 | EnableGrp1NS */
							/* EnableGrp1S | ARE_S | ARE_NS */
	str	w9, [x0, GICD_CTLR]		/* Secure GICD_CTLR */
	ret

