all:
	@PYTHONDONTWRITEBYTECODE=True scons
	qemu-system-riscv64 -nographic -machine virt -kernel rtthread.elf -bios none

clean:
	@PYTHONDONTWRITEBYTECODE=True scons -c
	rm -rf build rtthread.bin rtthread.elf rtthread.map

