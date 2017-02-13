#include <config.h>
#include <macro.h>


__armv8_dcache_all:
	mov	x1, x0
	dsb	ish
	mrs	x10, clidr_el1
	lsr	x11, x10, #24
	and	x11, x11, #0x7
	cbz	x11, finished
	mov	x15, x30
	mov	x0, #0

loop_level:
	lsl	x12, x0, #1
	add	x12, x12, x0
	lsr	x12, x10, x12
	and	x12, x12, #7
	cmp	x12, #2
	b.lt	skip
	bl	__armv8_flush_dcache_level	/* x1 = 0 flush, 1 invalidate */
skip:
	add	x0, x0, #1					/* increment cache level */
	cmp	x11, x0
	b.gt	loop_level

	mov	x0, #0
	msr	csselr_el1, x0				/* restore csselr_el1 */
	dsb	ish
	isb

	mov	x30, x15
finished:
	ret


__armv8_flush_dcache_level:
	lsl	x12, x0, #1
	msr	csselr_el1, x12
	isb
	mrs	x6, ccsidr_el1
	and	x2, x6, #7				/* x2 <- log2(cache line size) - 4 */
	add	x2, x2, #4				/* x2 <- log2(cache line size) */
	mov	x3, #0x3ff
	and	x3, x3, x6, lsr #3		/* x3 <- max number of #ways */
	clz	w5, w3					/* x5 = 32 - log2(ways) bit position of #ways */
	mov	x4, #0x7fff
	and	x4, x4, x6, lsr #13		/* x4 <- max number of #sets */

loop_set:
	mov	x6, x3
loop_way:
	lsl	x7, x6, x5
	orr	x9, x12, x7
	lsl	x7, x4, x2
	orr	x9, x9, x7
	tbz	w1, #0, 1f
	dc	isw, x9
	b	2f
1:	dc	cisw, x9
2:	subs	x6, x6, #1
	b.ge	loop_way
	subs	x4, x4, #1
	b.ge	loop_set
	ret


.global set_sctlr
set_sctlr:
	mov x29, x30
	armv8_switch_el x1, 3f, 2f, 1f
3:
  msr sctlr_el3, x0
  b 0f
2:
  msr sctlr_el2, x0
  b 0f
1:
  msr sctlr_el1, x0
0:
	mov x30, x29
	ret

.global get_sctlr
get_sctlr:
	mov x29, x30

	armv8_switch_el x1, 3f, 2f, 1f
3:
  mrs x0, sctlr_el3
  b 0f
2:
  mrs x0, sctlr_el2
  b 0f
1:
  mrs x0 ,sctlr_el1
0:
	mov x0, x0
	mov x30, x29
	ret

.global _invalidate_icache_all
_invalidate_icache_all:
	ic ialluis
	isb
	ret

.global _flush_dcache_all
_flush_dcache_all:
	mov x29, x30

	mov x0, #0
	bl __armv8_dcache_all

	ic ialluis
	isb

	mov x30, x29
	ret

.global _invalidate_dcache_all
_invalidate_dcache_all:
	mov x29, x30

	mov x0, #0xffff
	bl __armv8_dcache_all

	ic ialluis
	isb

	mov x30, x29
	ret


.global _invalidate_tlb_all
_invalidate_tlb_all:
	tlbi alle3
	dsb ish
	isb
	ret
