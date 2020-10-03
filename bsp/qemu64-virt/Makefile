all:
	@scons
	qemu-system-riscv64 -nographic -machine virt -kernel rtthread.elf -bios none

clean:
	@scons -c
