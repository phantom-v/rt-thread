all:
	@scons
	qemu-system-riscv32 -nographic -machine sifive_e -kernel rtthread.elf -bios none

clean:
	@scons -c
