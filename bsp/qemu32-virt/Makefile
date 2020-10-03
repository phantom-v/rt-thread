all:
	@scons
	qemu-system-riscv32 -nographic -machine virt -kernel rtthread.elf -bios none

clean:
	@scons -c
