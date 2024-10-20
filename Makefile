# Config

OUT_DIR := build
SRC_DIR := src
CC := x86_64-elf-gcc
CC_FLAGS := -ffreestanding -c -m32
LD := x86_64-elf-ld
LD_FLAGS := --oformat binary -m elf_i386
ASM := nasm
ASM_ELF_FLAGS := -f elf -i$(SRC_DIR)
ASM_BIN_FLAGS := -f bin -i$(SRC_DIR)

BOOTLOADER_SOURCES := $(SRC_DIR)/start.S $(SRC_DIR)/gdt.S $(SRC_DIR)/pm_switch.S
KERNEL_SOURCES  := $(patsubst %, $(SRC_DIR)/%, kernel_start.S kmain.c)
BOOTLOADER_BIN := $(OUT_DIR)/bootloader.bin
KERNEL_BIN := $(OUT_DIR)/kernel.bin
IMG := $(OUT_DIR)/ssos.img

# Auto-generated vars

KERNEL_OBJS := $(patsubst $(SRC_DIR)/%, $(OUT_DIR)/%.o, $(KERNEL_SOURCES))

# Rules
.PHONY: all clean

all: $(IMG)

# Compile bootloader assembly files directly into bootloader.bin
$(BOOTLOADER_BIN): $(BOOTLOADER_SOURCES)
	@mkdir -p $(OUT_DIR)
	$(ASM) $(ASM_BIN_FLAGS) -o $@ $(SRC_DIR)/start.S

# Compile kernel assembly file with -f elf
$(OUT_DIR)/%.S.o: $(SRC_DIR)/%.S
	@mkdir -p $(OUT_DIR)
	$(ASM) $(ASM_ELF_FLAGS) -o $@ $<

# Compile kernel C files
$(OUT_DIR)/%.c.o: $(SRC_DIR)/%.c
	@mkdir -p $(OUT_DIR)
	$(CC) $(CC_FLAGS) -o $@ $<

# Link kernel objects into a binary
$(KERNEL_BIN): $(KERNEL_OBJS)
	$(LD) $(LD_FLAGS) -o $@ $^

# Combine bootloader.bin and kernel.bin into final image
$(IMG): $(BOOTLOADER_BIN) $(KERNEL_BIN)
	cat $^ > $@

clean:
	rm -rf $(OUT_DIR)/*
