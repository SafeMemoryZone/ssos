SRC_DIR := src
KERNEL_SRC_DIR := $(SRC_DIR)/kernel
BOOTLOADER_SRC_DIR := $(SRC_DIR)/start
BUILD_DIR := build

ASM := nasm
ASM_FLAGS := -f elf -i $(SRC_DIR)/
CC := x86_64-elf-gcc
CC_FLAGS := -ffreestanding -c
LD := x86_64-elf-ld
LD_FLAGS := --oformat binary -Ttext 0x1000

KERNEL_SOURCES := $(wildcard $(KERNEL_SRC_DIR)/*.c)
BOOTLOADER_SOURCES := $(wildcard $(BOOTLOADER_SRC_DIR)/*.S)

KERNEL_OBJECTS := $(patsubst $(KERNEL_SRC_DIR)/%.c, $(BUILD_DIR)/kernel_%.o, $(KERNEL_SOURCES))
BOOTLOADER_OBJECTS := $(patsubst $(BOOTLOADER_SRC_DIR)/%.S, $(BUILD_DIR)/bootloader_%.o, $(BOOTLOADER_SOURCES))

KERNEL_BIN := $(BUILD_DIR)/kernel.bin
BOOTLOADER_BIN := $(BUILD_DIR)/bootloader.bin

.PHONY: all clean

all: $(BOOTLOADER_BIN) $(KERNEL_BIN)

$(BUILD_DIR)/bootloader_%.o: $(BOOTLOADER_SRC_DIR)/%.S
	mkdir -p $(BUILD_DIR)
	$(ASM) $(ASM_FLAGS) -o $@ $<

$(BUILD_DIR)/kernel_%.o: $(KERNEL_SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CC_FLAGS) -o $@ $<

$(BOOTLOADER_BIN): $(BOOTLOADER_OBJECTS)
	$(LD) $(LD_FLAGS) -o $@ $^

$(KERNEL_BIN): $(KERNEL_OBJECTS)
	$(LD) $(LD_FLAGS) -o $@ $^

clean:
	rm -rf $(BUILD_DIR)/*
