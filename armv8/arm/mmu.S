/*
 * We use the 4KB granule size, both IA(VA) and OA(PA) are 32bits width.
 *
 * Because IA(VA) is 32 bits(4GB), we don't need to use Level 0 and Level 3,
 * and, we only need 4 entries in Level 1 page table, 1GB per entry.
 *
 * the first 3 entries of Level 1 table are table descriptor which contains
 * the Level 2 page table address.
 * the last entry of Level 1 table is a block descriptor which describes 1GB
 * block size, and used for peripheral device address space.
 *
 * Level 2 (the final level) has 3 page tables in total, each entry of Level 2
 * page table contains block descriptor, 2MB per entry.
 */

#define MMU_PAGE_SIZE			0x1000

/* TCR_EL3:
* 4GB Physical Address, 4KB granule size
 * Inner shareable, Outer cacheability WBWA, Inner cacheability WBWA */
#define TCR_FLAGS			0x00003520

/* MAIR_EL3:
 * Attr0: nGnRnE device, Attr1: Normal memory, Attr2~Attr7: not used */
#define MEMORY_ATTRIBUTES  		0x0000ff00

/* memory block properties, use Attr1, Inner Shareable, turn on bit 10 - AF */
#define	MEMORY_PROPERTIES		((1 << 10) | (3 << 8) | (1 << 2) | 0x1)
/* device block properties, use Attr0, turn on bit 10 - AF */
#define	DEVICE_PROPERTIES		((1 << 54) | (1 << 10) | (0 << 2) | 0x1)
/* default memory limitation 3GB */
#define MEMORY_ADDR_LIMITED			(3 << 30)
#define DRAM_SIZE				(0x40000000)

.global mmu_init
mmu_init:
	ldr	x10, pagetable_level1
	ldr	x11, pagetable_level2

	/* initialize level 1 page table */
	orr	x0, x11, #0x3		/* 0x3 is table descriptor */
	str	x0, [x10]		/* point to level2 table 0 */
	add	x0, x0, #MMU_PAGE_SIZE
	str	x0, [x10, #8]		/* point to level2 table 1 */
	add	x0, x0, #MMU_PAGE_SIZE
	str	x0, [x10, #16]		/* point to level2 table 2 */

	ldr	x0, =DEVICE_PROPERTIES
	orr	x0, x0, #(3 << 30)
	str	x0, [x10, #24]		/* block descriptor: 3GB ~ 4GB */

	/* now initialize level 2 page table */
	mov	x9, #(DRAM_SIZE >> 21)					/* x9 = dram size in 2MB unit */
	ldr	x8, =MEMORY_PROPERTIES
	mov	x6, #(MEMORY_ADDR_LIMITED >> 21)		/* x6 = memory limitation 3GB */
	mov	x5, #0                                  /* x5 = index for each 2MB block */

	/* setup all the 3G memory */
1:	lsl	x1, x5, #21
	orr	x1, x1, x8
	cmp	x5, x9
	csel	x0, x1, xzr, lt		/* clear entry if out of dram size */
	str	x0, [x11]
	add	x11, x11, #8
	add	x5, x5, #1
	cmp	x5, x6
	b.lt	1b

2:
	msr	ttbr0_el3, x10

	ldr	x0, =TCR_FLAGS
	msr	tcr_el3, x0

	ldr	x0, =MEMORY_ATTRIBUTES
	msr	mair_el3, x0

	mrs	x0, sctlr_el3
	orr	x0, x0, #0x1	/* enable MMU */
	orr	x0, x0, #0x4	/* enable dcache */
	orr	x0, x0, #(1 << 12)	/* enable icache */
	msr	sctlr_el3, x0

	ret

.align 3
pagetable_level1:	.quad	__pagetable_level1
pagetable_level2:	.quad	__pagetable_level2


