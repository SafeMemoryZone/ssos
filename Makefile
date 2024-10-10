ASM = nasm
ASM_FLAGS = -f bin -isrc/

.PHONY: all clean

all: bootloader

bootloader:
	$(ASM) $(ASM_FLAGS) src/start.S -o build/start.bin

clean:
	rm -rf build/*
