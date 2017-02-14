#include "common.h"
#include "arm-gic-v3.h"

#define GICD_BASE   0xf6801000
#define GICR_BASE   0x5a5a5a5a
#define GICC_BASE   0xf6802000

#define GICD_REG(x)  (GICD_BASE + (x))
#define GICR_REG(x)  (GICR_BASE + (x))
#define GICC_REG(x)  (GICC_BASE + (x))

struct gic_info
{
	unsigned int irq_nr;
};

void gic_irq_type(int irq, int type)
{
	unsigned int value = 0;
	unsigned int typemask = 0x2 << ((irq % 16) * 2);

	value = read32(GICD_REG(GICD_ICFGR) + (irq / 16 ) * 4);

	if (type)
		value |= typemask;
	else
		value &= ~typemask;

	write32(GICD_REG(GICD_ICFGR) + (irq / 16 ) * 4, value);
}

void gic_disable_irq(int irq)
{
	write32(GICD_REG(GICD_ICENABLER) + (irq / 32) * 4 , 1 << (irq % 32));
}

void gic_enable_irq(int irq, int type)
{
	gic_irq_type(irq, type);
	write32(GICD_REG(GICD_ISENABLER) + (irq / 32) * 4 , 1 << (irq % 32));
}

void gic_dist_reg(int reg)
{
	unsigned int value = 0;

	value = read32(GICD_REG(reg));
	print32hex(reg);
	print32hex(value);
}

void gic_cpu_reg(int reg)
{
	unsigned int value = 0;

	value = read32(GICC_REG(reg));
	print32hex(reg);
	print32hex(value);
}
void gic_dist_debug(int irq)
{
	gic_dist_reg(GICD_CTLR		);
	gic_dist_reg(GICD_TYPER		);
	gic_dist_reg(GICD_IIDR		);
	gic_dist_reg(GICD_STATUSR	);
	gic_dist_reg(GICD_SETSPI_NSR);
	gic_dist_reg(GICD_CLRSPI_NSR);
	gic_dist_reg(GICD_SETSPI_SR	);
	gic_dist_reg(GICD_CLRSPI_SR	);
	gic_dist_reg(GICD_SEIR		);
	gic_dist_reg(GICD_IGROUPR	+ (irq / 32) * 4);
	gic_dist_reg(GICD_ISENABLER	+ (irq / 32) * 4);
	gic_dist_reg(GICD_ICENABLER	+ (irq / 32) * 4);
	gic_dist_reg(GICD_ISPENDR + (irq / 32) * 4);
	gic_dist_reg(GICD_ICPENDR + (irq / 32) * 4);
	gic_dist_reg(GICD_ISACTIVER + (irq / 32) * 4	);
	gic_dist_reg(GICD_ICACTIVER + (irq / 32) * 4	);
	gic_dist_reg(GICD_IPRIORITYR + (irq / 4) * 4 );
	gic_dist_reg(GICD_ICFGR		);
	gic_dist_reg(GICD_IGRPMODR	);
	gic_dist_reg(GICD_NSACR		);
	gic_dist_reg(GICD_IROUTER	);
	gic_dist_reg(GICD_SPENDSGIR	);
	gic_dist_reg(GICD_CPENDSGIR );
}

void gic_cpu_debug(void)
{
	gic_cpu_reg(GIC_CPU_CTRL			);
	gic_cpu_reg(GIC_CPU_PRIMASK			);
	gic_cpu_reg(GIC_CPU_BINPOINT		);
	gic_cpu_reg(GIC_CPU_INTACK			);
	gic_cpu_reg(GIC_CPU_RUNNINGPRI		);
	gic_cpu_reg(GIC_CPU_HIGHPRI			);
	gic_cpu_reg(GIC_CPU_ALIAS_BINPOINT	);
	gic_cpu_reg(GIC_CPU_AIAR		    );
	gic_cpu_reg(GIC_CPU_AHPP		    );
	gic_cpu_reg(GIC_CPU_ACTIVEPRIO		);
	gic_cpu_reg(GIC_CPU_DIR			);

}


void gic_debug(int irq)
{
	//gic_dist_debug(irq);
	gic_cpu_debug();
}

void handle_sgi(int irq)
{
	//write32(GICD_REG(GICD_CPENDSGIR) + irq / 4, 1 << ((irq % 4) * 8));
}

void gic_handle_irq(void)
{
	unsigned int value = 0;
	unsigned int irq_nr = 0;
	value = read32(GICC_REG(GIC_CPU_INTACK));

	irq_nr = value & 0x3ff;
#if 0
	cpuid = ( value >> 10 ) & 0x7 ;

	printstr("CPUID:\t");
	print32hex(cpuid);
	printstr("IRQ:\t");
	print32hex(irq_nr);
#endif

	if (irq_nr < 16)
		handle_sgi(irq_nr);

	if (irq_nr == 46)
		timer_handle();


	write32(GICC_REG(GIC_CPU_EOI), value);
}

void gic_send_sgi(int cpumask, int irq)
{
	unsigned int value = 0;
	if (irq > 15)
		return ;

	value = (cpumask & 0xff) << 16;
	printstr("send SGI: \t");
	print32hex(irq);
	value |= (irq & 0xff);

	write32(GICD_REG(GICD_SGIR), value);
}
static int gic_dist_config(struct gic_info *gic)
{
	int i;
	/* interrupt: 0 : level-sen or 1: edge triggered */

	for (i = 32; i < gic->irq_nr; i += 16)
		write32(GICD_REG(GICD_ICFGR) + i / 4, 0x0);

	/* set priority, default 0xa0 higher than cpu priority 0xf0 */

	for (i = 32; i < gic->irq_nr; i += 4)
		write32(GICD_REG(GICD_IPRIORITYR) + i, 0xa0a0a0a0);

	/* disable all */
	for (i = 32; i < gic->irq_nr; i += 32)
		write32(GICD_REG(GICD_ICENABLER) + i / 8, ~0);

	/* wait for rwp : GICD_CTLR & GICD_ICENABLER */

	return 0;

}

static int gic_dist_target(struct gic_info *gic)
{
	int i;
	unsigned int cpumask = 0;   /* FIXME */

	cpumask = (0xff << 0) | (0xff << 8) | (0xff << 16) | (0xff << 24);
#if 1
	for (i = 32; i < gic->irq_nr; i += 4)
		write32(GICD_REG(GICD_ITARGETSR) + i , cpumask);
#else
	for (i = 32; i < gic->irq_nr; i++)
		writex(GICD_REG(GICD_IROUTER) + i * 8, cpumask);
#endif

	return 0;
}

static int gic_dist_group(struct gic_info *gic)
{
	int i;

	for (i = 32; i < gic->irq_nr; i += 32){
#if 0
		write32(GICD_REG(GICD_IGROUPR) + i / 8, ~0);
#else
		write32(GICD_REG(GICD_IGRPMODR) + i / 8, 0);
#endif
	}

	return 0;
}
static int gic_dist_sgi(struct gic_info *gic)
{

	int i;

	/*
	 * Deal with the banked PPI and SGI interrupts - disable all
	 * PPI interrupts, ensure all SGI interrupts are enabled.
	 */

	write32(GICD_REG(GICD_ICENABLER), 0xffff0000);
	write32(GICD_REG(GICD_ISENABLER), 0x0000ffff);

	/*
	 * Set priority on PPI and SGI interrupts
	 */
	for (i = 0; i < 32; i += 4)
		write32(GICD_REG(GICD_IPRIORITYR) + i, 0xa0a0a0a0);

	return 0;
}

static int gic_dist_init(struct gic_info *gic)
{
	write32(GICD_REG(GICD_CTLR), 0);

	/* wait for rwp : GICD_CTLR & GICD_ICENABLER */

	gic_dist_config(gic);
	gic_dist_target(gic);
#if 1
	gic_dist_group(gic);
#endif
	gic_dist_sgi(gic);

#if 1
	write32(GICD_REG(GICD_CTLR), 3);
#else
	write32(GICD_REG(GICD_CTLR), 0x37)    /* G0, G1S, G1NS, ARE_S, ARE_NS enable */
#endif

	/* wait for rwp : GICD_CTLR & GICD_ICENABLER */

	return 0;

}

static int gic_cpu_priceority_threshold(struct gic_info *gic)
{
#if 1
	write32(GICC_REG(GIC_CPU_PRIMASK), 0xf0);
#endif
	return 0;
}

static int gic_cpu_config(struct gic_info *gic)
{
	return 0;
}

static int gic_cpu_intface_enable(void)
{

	unsigned int bypass = 0;

	bypass = read32(GICC_REG(GIC_CPU_CTRL));
	bypass &= 0x1e0;

	write32(GICC_REG(GIC_CPU_CTRL), bypass | 0x1);

	return 0;
}

static int gic_cpu_init(struct gic_info *gic)
{
	gic_cpu_priceority_threshold(gic);
	gic_cpu_config(gic);
	gic_cpu_intface_enable();

	return 0;
}

int bootstrap_gic_init(void)
{
	struct gic_info gic;
	int irqline_nr;


	/* Get interrupts nr supported */
	irqline_nr = read32(GICD_REG(GICD_TYPER)) && 0x1f;

	gic.irq_nr = ((irqline_nr + 1) * 32);
	if (gic.irq_nr > 1020)
		gic.irq_nr = 1020;

	/* Distributor init (SPI)*/
	gic_dist_init(&gic);

	/* CPU interface init (SGI & PPI) */
	gic_cpu_init(&gic);

	return 0;
}
