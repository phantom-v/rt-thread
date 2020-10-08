all: run

run:
	@PYTHONDONTWRITEBYTECODE=True scons
	qemu-system-riscv64 -nographic -machine virt -kernel rtthread.elf -bios none

debug:
	@PYTHONDONTWRITEBYTECODE=True scons
	qemu-system-riscv64 -nographic -machine virt -kernel rtthread.elf -bios none -s -S

clean:
	@PYTHONDONTWRITEBYTECODE=True scons -c
	rm -rf build rtthread.bin rtthread.elf rtthread.map

