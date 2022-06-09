GENERATE = generate.out
OS_SECTOR = os_sector.bin
OS_SECTOR_DISASM = $(OS_SECTOR:.bin=.asm)

all: $(OS_SECTOR_DISASM) run

run: $(OS_SECTOR)
	qemu-system-x86_64 $(OS_SECTOR)

$(OS_SECTOR_DISASM): $(OS_SECTOR)
	ndisasm $< > $@

$(OS_SECTOR): $(GENERATE)
	./$<

$(GENERATE): stupid.c
	cc -o $@ $<

clean:
	-rm $(GENERATE)
	-rm $(OS_SECTOR)
	-rm $(OS_SECTOR_DISASM)
