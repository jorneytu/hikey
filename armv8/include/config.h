#ifndef __ARM_CONFIG__
#define __ARM_CONFIG__


#define AARCH64_SPSR_EL3h 0b1101
#define AARCH64_SPSR_EL3t 0b1100
#define AARCH64_SPSR_EL2h 0b1001
#define AARCH64_SPSR_EL2t 0b1000
#define AARCH64_SPSR_EL1h 0b0101
#define AARCH64_SPSR_EL1t 0b0100
#define AARCH64_SPSR_EL0t 0b0000
#define AARCH64_SPSR_RW (1 << 4)
#define AARCH64_SPSR_F  (1 << 6)
#define AARCH64_SPSR_I  (1 << 7)
#define AARCH64_SPSR_A  (1 << 8)
#define AARCH64_SPSR_D  (1 << 9)
#define AARCH64_SPSR_IL (1 << 20)
#define AARCH64_SPSR_SS (1 << 21)
#define AARCH64_SPSR_V  (1 << 28)
#define AARCH64_SPSR_C  (1 << 29)
#define AARCH64_SPSR_Z  (1 << 30)
#define AARCH64_SPSR_N  (1 << 31)

#define GICD_CTLR 			0x0000
#define GICD_TYPER 			0x0004
#define GICD_IGROUPRn 		0x0080
#define GICD_IGROUPMODRn 	0x0d00

#define SC_ICACHE_ENABLE   (1 << 12)
#define SC_DCACHE_ENABLE   (1 << 2)
#define SC_ALIGN_ENABLE    (1 << 1)
#define SC_MMU_ENABLE      (1 << 0)


#endif
