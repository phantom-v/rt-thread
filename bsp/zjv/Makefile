all: run

run:
	@PYTHONDONTWRITEBYTECODE=True scons
	spike rtthread.elf

debug:
	@PYTHONDONTWRITEBYTECODE=True scons
	spike -H --rbb-port=9824 rtthread.elf & sleep 1; openocd -f spike.cfg

clean:
	@PYTHONDONTWRITEBYTECODE=True scons -c
	rm -rf build rtthread.bin rtthread.elf rtthread.map rtthread.bin.txt

