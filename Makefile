all:arm startup

startup:
	@make -s -C boot

arm:
	@make -s -C armv8

clean:
	rm bootstrap.bin -rf
	make -C armv8 clean
	make -C boot clean
