include config.mk
OBJS := arm/start.o
OBJS += arm/mmu.o
OBJS += arm/vector.o
OBJS += arm/bandwidth.o

OBJS += lib/main.o
OBJS += lib/misc.o
OBJS += lib/irq.o
OBJS += lib/aarch64.o
OBJS += lib/gic.o
OBJS += lib/timer.o
OBJS += lib/watchdog.o


all: base
	$(LD) $(OBJS) -T cortex-a53.lds -Map cortex-a53.map -o lucifer.elf
	$(Q)$(call SYSTEM_MAP, lucifer.elf)> System.map
	$(Q)$(COPY) --gap-fill=0xff -O binary lucifer.elf lucifer.bin
	$(Q)echo "aarch64 binary build ..."

base: $(OBJS)

%.o:%.S
	$(Q)$(CC) $(CFLAGS) -c $< -o $@

%.o:%.c
	$(Q)$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(Q)rm $(OBJS) -rf
	$(Q)rm lucifer* *.map -rf

