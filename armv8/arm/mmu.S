
#include <config.h>
#include <mmu.h>

.global mmu_enable
mmu_enable:

	mov x29, x30
	ldr x0, pgtable_start
	ldr x1, =PAGE_ENTRIES
level0:
	add x0, x0, #0
	bl pre_clear

	add x2, x0, #PAGE_SIZE
	orr x2, x2, #PMD_TYPE_TABLE

	add x0, x0, #0
	str x2, [x0]


level1:
	add x0, x0, #PAGE_SIZE
	ldr x1, =PAGE_ENTRIES
	bl pre_clear

	/* 0 - 1G for mem */
	add x0, x0, #0
	ldr x2, =(0 << 30)
	ldr x3, =(PMD_SECT_AF | PMD_TYPE_SECT | PMD_ATTRINDX(MT_NORMAL))
	orr x2, x2, x3
	str x2, [x0]

	/* 1 - 4G for peripheral */
	add x0, x0, #8
	ldr x2, =(1 << 30)
	ldr x3, =(PMD_SECT_AF | PMD_TYPE_SECT | PMD_ATTRINDX(MT_DEVICE_NGNRNE))
	orr x2, x2, x3
	str x2, [x0]

	add x0, x0, #8
	ldr x2, =(2 << 30)
	ldr x3, =(PMD_SECT_AF | PMD_TYPE_SECT | PMD_ATTRINDX(MT_DEVICE_NGNRNE))
	orr x2, x2, x3
	str x2, [x0]

	add x0, x0, #8
	ldr x2, =(3 << 30)
	ldr x3, =(PMD_SECT_AF | PMD_TYPE_SECT | PMD_ATTRINDX(MT_DEVICE_NGNRNE))
	orr x2, x2, x3
	str x2, [x0]

finish:
	ldr x0, pgtable_start
	msr ttbr0_el3, x0

	ldr x0, =PAGE_4K_TCR
	msr tcr_el3, x0

	ldr x0, =MEMORY_ATTRIBUTES
	msr mair_el3, x0

	mrs x0, sctlr_el3
	orr x0, x0, SC_MMU_ENABLE
	msr sctlr_el3, x0

	mov x30, x29
	ret





pre_clear:
	mov x2, x0
	mov x3, x1
loop:
	str xzr, [x2]
	sub x3, x3, #1
	add x2, x2, #8
	cbnz x3, loop

	ret

.global mmu_enable_64K
mmu_enable_64K:
	mov x29, x30
	ldr x0, pgtable_start
	ldr x1, =PAGE_ENTRIES
_level0:
_level1:
	add x0, x0, #0
	bl pre_clear

	add x2, x0, #PAGE_SIZE
	orr x2, x2, #PMD_TYPE_TABLE
	str x2, [x0]
_level2:
	add x0, x0, #PAGE_SIZE
	ldr x1, =PAGE_ENTRIES
	bl pre_clear


	/* 0 - 1G */
	add x0, x0, #0
	ldr x2, =(0 << 29)
	ldr x3, =(PMD_SECT_AF | PMD_TYPE_SECT | PMD_ATTRINDX(MT_NORMAL))
	orr x2, x2, x3
	str x2, [x0]

	add x0, x0, #8
	ldr x2, =(1 << 29)
	ldr x3, =(PMD_SECT_AF | PMD_TYPE_SECT | PMD_ATTRINDX(MT_NORMAL))
	orr x2, x2, x3
	str x2, [x0]


	/* 1G - 4G */
	add x0, x0, #8
	ldr x2, =(2 << 29)
	ldr x3, =(PMD_SECT_AF | PMD_TYPE_SECT | PMD_ATTRINDX(MT_DEVICE_NGNRNE))
	orr x2, x2, x3
	str x2, [x0]

	add x0, x0, #8
	ldr x2, =(3 << 29)
	ldr x3, =(PMD_SECT_AF | PMD_TYPE_SECT | PMD_ATTRINDX(MT_DEVICE_NGNRNE))
	orr x2, x2, x3
	str x2, [x0]

	add x0, x0, #8
	ldr x2, =(4 << 29)
	ldr x3, =(PMD_SECT_AF | PMD_TYPE_SECT | PMD_ATTRINDX(MT_DEVICE_NGNRNE))
	orr x2, x2, x3
	str x2, [x0]

	add x0, x0, #8
	ldr x2, =(5 << 29)
	ldr x3, =(PMD_SECT_AF | PMD_TYPE_SECT | PMD_ATTRINDX(MT_DEVICE_NGNRNE))
	orr x2, x2, x3
	str x2, [x0]

	add x0, x0, #8
	ldr x2, =(6 << 29)
	ldr x3, =(PMD_SECT_AF | PMD_TYPE_SECT | PMD_ATTRINDX(MT_DEVICE_NGNRNE))
	orr x2, x2, x3
	str x2, [x0]

	add x0, x0, #8
	ldr x2, =(7 << 29)
	ldr x3, =(PMD_SECT_AF | PMD_TYPE_SECT | PMD_ATTRINDX(MT_DEVICE_NGNRNE))
	orr x2, x2, x3
	str x2, [x0]

_finish:
	ldr x0, pgtable_start
	msr ttbr0_el3, x0

	ldr x0, =PAGE_64K_TCR
	msr tcr_el3, x0

	ldr x0, =MEMORY_ATTRIBUTES
	msr mair_el3, x0

#if 1
	mrs x0, sctlr_el3
	orr x0, x0, SC_MMU_ENABLE
	msr sctlr_el3, x0
#endif

	mov x30, x29
	ret
