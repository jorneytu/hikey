#ifndef __ASM_ARM_MACRO_H__
#define __ASM_ARM_MACRO_H__
#ifdef __ASSEMBLY__

.macro	armv8_switch_el, xreg, el3_label, el2_label, el1_label
	mrs	\xreg, CurrentEL
	cmp	\xreg, 0xc
	b.eq	\el3_label
	cmp	\xreg, 0x8
	b.eq	\el2_label
	cmp	\xreg, 0x4
	b.eq	\el1_label
.endm

#endif /* __ASSEMBLY__ */

#endif /* __ASM_ARM_MACRO_H__ */

