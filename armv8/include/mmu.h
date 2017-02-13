#ifndef __AARCH64_MMU__
#define __AARCH64_MMU__

#define PAGE_SIZE 0x1000

#ifdef __ASSEMBLY__
#define _AC(X, Y)	X
#else
#define _AC(X, Y)	(X##Y)
#endif

#define UL(x)		_AC(x, UL)

#if  PAGE_SIZE == 0x4000
#define PAGE_ENTRIES   0x800
#define SECTION_SHIFT_L0	47UL    /* 128T */
#define SECTION_SHIFT_L1	36UL    /* 64G  */
#define SECTION_SHIFT_L2	25UL    /* 32M  */
#define SECTION_SHIFT_L3	14UL    /* 16K  */
#elif PAGE_SIZE == 0x10000
#define PAGE_ENTRIES   0x2000
#define SECTION_SHIFT_L0	42UL    /* 4T   */
#define SECTION_SHIFT_L1	29UL    /* 512M */
#define SECTION_SHIFT_L2	16UL    /* 64K  */
#else
#define PAGE_ENTRIES   0x200
#define SECTION_SHIFT_L0	39UL    /* 512G */
#define SECTION_SHIFT_L1	30UL    /* 1G   */
#define SECTION_SHIFT_L2	21UL    /* 2M   */
#define SECTION_SHIFT_L3	12UL    /* 4K   */
#endif

#define PMD_TYPE_MASK		(3 << 0)
#define PMD_TYPE_FAULT		(0 << 0)
#define PMD_TYPE_TABLE		(3 << 0)
#define PMD_TYPE_SECT		(1 << 0)

#define PMD_SECT_S			(3 << 8)
#define PMD_SECT_AF			(1 << 10)
#define PMD_SECT_NG			(1 << 11)
#define PMD_SECT_PXN		(UL(1) << 53)
#define PMD_SECT_UXN		(UL(1) << 54)

#define PMD_ATTRINDX(t)		((t) << 2)
#define PMD_ATTRINDX_MASK	(7 << 2)

#define MT_DEVICE_NGNRNE	0
#define MT_DEVICE_NGNRE		1
#define MT_DEVICE_GRE		2
#define MT_NORMAL_NC		3
#define MT_NORMAL			4


#if 0
#define MEMORY_ATTRIBUTES	((0x00 << (MT_DEVICE_NGNRNE*8)) |	\
				(0x04 << (MT_DEVICE_NGNRE*8)) |		\
				(0x0c << (MT_DEVICE_GRE*8)) |		\
				(0x44 << (MT_NORMAL_NC*8)) |		\
				(0xffUL << (MT_NORMAL*8)))
#endif
#define MEMORY_ATTRIBUTES  0xff440c0400

#define VA_BITS				(40)	/* 42 bits virtual address */

#define TCR_T0SZ(x)			((64 - (x)) << 0)

#define TCR_IRGN_NC			(0 << 8)
#define TCR_IRGN_WBWA		(1 << 8)
#define TCR_IRGN_WT			(2 << 8)
#define TCR_IRGN_WBNWA		(3 << 8)
#define TCR_IRGN_MASK		(3 << 8)

#define TCR_ORGN_NC			(0 << 10)
#define TCR_ORGN_WBWA		(1 << 10)
#define TCR_ORGN_WT			(2 << 10)
#define TCR_ORGN_WBNWA		(3 << 10)
#define TCR_ORGN_MASK		(3 << 10)

#define TCR_SHARED_NON		(0 << 12)
#define TCR_SHARED_OUTER	(1 << 12)
#define TCR_SHARED_INNER	(2 << 12)

#define TCR_TG0_4K			(0 << 14)
#define TCR_TG0_64K			(1 << 14)
#define TCR_TG0_16K			(2 << 14)

#define TCR_EL1_IPS_BITS	(3UL << 32)		/* 42 bits physical address */
#define TCR_EL2_IPS_BITS	(3 << 16)		/* 42 bits physical address */
//#define TCR_EL3_IPS_BITS	(3 << 16)		/* 42 bits physical address */

#define TCR_EL3_IPS_BITS    (2 << 16)


/* PTWs cacheable, inner/outer WBWA and non-shareable */
#define PAGE_64K_TCR		(TCR_TG0_64K |		\
				TCR_EL3_IPS_BITS | \
				TCR_SHARED_NON |	\
				TCR_ORGN_WBWA |		\
				TCR_IRGN_WBWA |		\
				TCR_T0SZ(VA_BITS))


#define BLOCK_SIZE_L0		(1UL << SECTION_SHIFT_L0)
#define BLOCK_SIZE_L1		(1 << SECTION_SHIFT_L1)
#define BLOCK_SIZE_L2		(1 << SECTION_SHIFT_L2)

#define TCR_EL2_PS_40BIT	(2 << 16)
#define PAGE_4K_VA_BITS		(40)

#if 1
#define PAGE_4K_TCR	(TCR_TG0_4K		| \
			TCR_EL2_PS_40BIT	| \
			TCR_SHARED_NON		| \
			TCR_ORGN_WBWA |		\
			TCR_IRGN_WBWA |		\
			TCR_T0SZ(PAGE_4K_VA_BITS))

#else
#define PAGE_4K_TCR	(TCR_TG0_4K		| \
			TCR_EL2_PS_40BIT	| \
			TCR_SHARED_NON		| \
			TCR_ORGN_NC |		\
			TCR_IRGN_NC |		\
			TCR_T0SZ(PAGE_4K_VA_BITS))
#endif



#endif
