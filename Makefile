OUT_DIR := build
SRC_DIR := src
OBJS_DIR := $(OUT_DIR)/objs
BIN_DIR := $(OUT_DIR)/bin
CC := x86_64-elf-gcc
CC_FLAGS := -ffreestanding -c -m32
LD := x86_64-elf-ld
LD_FLAGS := --oformat binary -m elf_i386
ASM := nasm
ASM_ELF_FLAGS := -f elf -i$(SRC_DIR)
ASM_BIN_FLAGS := -f bin -i$(SRC_DIR)

BOOTLOADER_SOURCES := $(SRC_DIR)/start.S $(SRC_DIR)/gdt.S $(SRC_DIR)/pm_switch.S
KERNEL_SOURCES  := $(SRC_DIR)/kernel_start.S $(SRC_DIR)/kmain.c
BOOTLOADER_BIN := $(BIN_DIR)/bootloader.bin
KERNEL_BIN := $(BIN_DIR)/kernel.bin
IMG := $(OUT_DIR)/ssos.img

KERNEL_OBJS := $(patsubst $(SRC_DIR)/%, $(OBJS_DIR)/%.o, $(KERNEL_SOURCES))

.PHONY: all clean

all: $(IMG)

$(BOOTLOADER_BIN): $(BOOTLOADER_SOURCES)
	@mkdir -p $(BIN_DIR)
	$(ASM) $(ASM_BIN_FLAGS) -o $@ $(SRC_DIR)/start.S

$(OBJS_DIR)/%.S.o: $(SRC_DIR)/%.S
	@mkdir -p $(OBJS_DIR)
	$(ASM) $(ASM_ELF_FLAGS) -o $@ $<

$(OBJS_DIR)/%.c.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJS_DIR)
	$(CC) $(CC_FLAGS) -o $@ $<

$(KERNEL_BIN): $(KERNEL_OBJS)
	@mkdir -p $(BIN_DIR)
	$(LD) $(LD_FLAGS) -o $@ $^

$(IMG): $(BOOTLOADER_BIN) $(KERNEL_BIN)
	@mkdir -p $(OUT_DIR)
	cat $^ > $@

clean:
	rm -rf $(OUT_DIR)/*
