#include <macro.h>
.text
.align 11

.global _vectors

_vectors:
//
// Current EL with SP0
//
_do_bad_sync:
	b _do_bad_sync_handle
	.balign 0x80
_do_bad_irq:
	b _do_bad_irq_handle
	.balign 0x80
_do_bad_fiq:
	b _do_bad_fiq_handle
	.balign 0x80
_do_bad_error:
	b _do_bad_error_handle
	.balign 0x80

//
// Current EL with SPx
//

do_sync:
	b do_sync_handle
	.balign 0x80
do_irq:
	b do_irq_handle
	.balign 0x80
do_fiq:
	b do_fiq_handle
	.balign 0x80
do_error:
	b do_error_handle
	.balign 0x80
//
// lower EL using AArch64 (lower exception switch to high)
//
l64_do_sync:
	b l64_sync_handle
	.balign 0x80
l64_do_irq:
	b l64_irq_handle
	.balign 0x80
l64_do_fiq:
	b l64_fiq_handle
	.balign 0x80
l64_do_error:
	b l64_error_handle
	.balign 0x80
//
// lower EL using AArch32 (lower exception switch to high)
//
l32_do_sync:
	b l32_do_sync
	.balign 0x80
l32_do_irq:
	b l32_do_irq
	.balign 0x80
l32_do_fiq:
	b l32_do_fiq
	.balign 0x80
l32_do_error:
	b l32_do_error
	.balign 0x80

