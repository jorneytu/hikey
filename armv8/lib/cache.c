#include <config.h>
#include <common.h>
#include <mmu.h>
#include <misc.h>

extern unsigned long long pgtable_start;

void pgtable_sector_setup(unsigned long long *page_table, unsigned long long index,
		unsigned long long section, unsigned long long memory_type)
{

	unsigned long long value;

	value = section | PMD_TYPE_SECT | PMD_SECT_AF;
	value |= PMD_ATTRINDX(memory_type);
	page_table[index] = value;

}
static void set_ttbr_tcr_mair(int el, u64 table, u64 tcr, u64 attr)
{
	asm volatile("dsb sy");
	if (el == 1) {
		asm volatile("msr ttbr0_el1, %0" : : "r" (table) : "memory");
		asm volatile("msr tcr_el1, %0" : : "r" (tcr) : "memory");
		asm volatile("msr mair_el1, %0" : : "r" (attr) : "memory");
	} else if (el == 2) {
		asm volatile("msr ttbr0_el2, %0" : : "r" (table) : "memory");
		asm volatile("msr tcr_el2, %0" : : "r" (tcr) : "memory");
		asm volatile("msr mair_el2, %0" : : "r" (attr) : "memory");
	} else if (el == 3) {
		asm volatile("msr ttbr0_el3, %0" : : "r" (table) : "memory");
		asm volatile("msr tcr_el3, %0" : : "r" (tcr) : "memory");
		asm volatile("msr mair_el3, %0" : : "r" (attr) : "memory");
	} else {
	}
	asm volatile("isb");
}
static void mmu_setup(void)
{
	u64 i;
	u64 section_l1t0;
	u64 *page_table = ( u64 *)pgtable_start;
	u64 *level0_table = (u64 *)page_table;
	u64 *level1_table_0 = (u64 *)(page_table + 0x1000);


	level0_table[0] =
		(u64)level1_table_0 | PMD_TYPE_TABLE;

	section_l1t0 = 0;
	for (i = 0; i < 512; i++) {
		pgtable_sector_setup(level1_table_0, i, section_l1t0,
				    MT_DEVICE_NGNRNE);
		section_l1t0 += BLOCK_SIZE_L1;
	}

	level1_table_0[0] =
		0x00000000 | PMD_SECT_AF | PMD_TYPE_SECT |
		PMD_ATTRINDX(MT_NORMAL);
	level1_table_0[1] =
		0x40000000 | PMD_SECT_AF | PMD_TYPE_SECT |
		PMD_ATTRINDX(MT_DEVICE_NGNRNE);
	level1_table_0[2] =
		0x80000000 | PMD_SECT_AF | PMD_TYPE_SECT |
		PMD_ATTRINDX(MT_DEVICE_NGNRNE);
	level1_table_0[3] =
		0xc0000000 | PMD_SECT_AF | PMD_TYPE_SECT |
		PMD_ATTRINDX(MT_DEVICE_NGNRNE);


	set_ttbr_tcr_mair(3, (u64)level0_table, PAGE_4K_TCR, MEMORY_ATTRIBUTES);
	set_sctlr(get_sctlr() | SC_MMU_ENABLE);
}

void icache_enable(void)
{
	set_sctlr(get_sctlr() | SC_ICACHE_ENABLE);
}

void icache_disable(void)
{
	_invalidate_icache_all();

	set_sctlr(get_sctlr() & ~SC_ICACHE_ENABLE);
}

void invalidate_icache_all(void)
{
	_invalidate_icache_all();
}


void invalidate_dcache_all(void)
{
	_invalidate_dcache_all();
	_invalidate_icache_all();
}
void flush_dcache_all(void)
{
	_flush_dcache_all();

}
void invalidate_dcache_range(unsigned long start , unsigned long end)
{
}
void flush_dcache_range(unsigned long start , unsigned long end)
{
}

void dcache_enable(void)
{
	if (!(get_sctlr() & SC_MMU_ENABLE)){
		invalidate_dcache_all();
		_invalidate_tlb_all();
		mmu_setup();
	}
	set_sctlr(get_sctlr() | SC_DCACHE_ENABLE);
}

void dcache_disable(void)
{
	if (!((get_sctlr() & SC_DCACHE_ENABLE)))
		return ;

	set_sctlr(get_sctlr() & ~(SC_DCACHE_ENABLE | SC_MMU_ENABLE));

	flush_dcache_all();
	_invalidate_tlb_all();
}

void enable_caches(void)
{
	icache_enable();
	dcache_enable();
}
