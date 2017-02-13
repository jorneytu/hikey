PWD := $(shell pwd)
CROSS_COMPILE := aarch64-linux-gnu-

CC := $(CROSS_COMPILE)gcc
AR := $(CROSS_COMPILE)ar
LD := $(CROSS_COMPILE)ld
NM:= $(CROSS_COMPILE)nm
COPY := $(CROSS_COMPILE)objcopy

AFLAGS := -D__ASSEMBLY__
AFLAGS += -mstrict-align -ffunction-sections -fdata-sections -fno-common -ffixed-r9 -fno-common -ffixed-x18 -pipe -march=armv8-a
AFLAGS += -Wall -Wstrict-prototypes -Wno-format-security -fno-builtin -ffreestanding -Os -fno-stack-protector -g -fstack-usage -Wno-format-nonliteral
INCFLAGS :=-I$(PWD)/include
LDFLAGS := -Bstatic -T u-boot.lds
Q :=@
CFLAGS := $(INCFLAGS) $(AFLAGS)

SYSTEM_MAP = $(NM) $1| grep -v '\(compiled\)\|\(\.o$$\)\|\( [aUw] \)\|\(\.\.ng$$\)\|\(LASH[RL]DI\)'|LC_ALL=C sort

%.o:%c
	$(Q)$(CC) $(CFLAGS) -o $@ $< -c

export CC AR LD NM COPY Q CFLAGS PWD
