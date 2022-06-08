GENERATE = generate.out
OS_SECTOR = os_sector.bin

os_sector.asm: $(OS_SECTOR)
	ndisasm $< > $@

$(OS_SECTOR): $(GENERATE)
	./$<

$(GENERATE): stupid.c
	cc -o $@ $<
