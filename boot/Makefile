CROSS_COMPILE=arm-linux-gnueabihf-
CC=$(CROSS_COMPILE)gcc
LD=$(CROSS_COMPILE)ld
OBJCOPY=$(CROSS_COMPILE)objcopy

OBJ	:= start.o debug.o aarch64.o

all: l-loader.bin

l-loader.bin: $(OBJ)
	$(LD) -Bstatic -Tl-loader.lds -Ttext 0x07000000 $(OBJ) -o loader.elf
	$(OBJCOPY) -O binary loader.elf $@
	@echo "aarch32 binary build ..."

%.o: %.S
	@$(CC) -c $<

clean:
	rm -rf *.o *.bin *.elf

