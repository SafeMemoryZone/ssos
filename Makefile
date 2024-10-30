OUT_DIR := build
SRC_DIR := src
OBJS_DIR := $(OUT_DIR)/objs
BIN_DIR := $(OUT_DIR)/bin
INCLUDE_DIR := include
CC := x86_64-elf-gcc
CFLAGS := -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
	  -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c \
	  -I$(INCLUDE_DIR)/drivers -I$(INCLUDE_DIR)/lib -I$(INCLUDE_DIR)/kernel -std=c11

LD := x86_64-elf-ld
LDFLAGS := -T $(SRC_DIR)/kernel/link.ld
AS := nasm
AS_ELF_FLAGS := -f elf -i$(INCLUDE_DIR)/bootloader
AS_BIN_FLAGS := -f bin -i$(INCLUDE_DIR)/bootloader

BOOTLOADER_SOURCES := $(wildcard $(SRC_DIR)/bootloader/*.S) \
		      $(wildcard $(SRC_DIR)/bootloader/*.c)

KERNEL_SOURCES := $(wildcard $(SRC_DIR)/kernel/*.S) \
		  $(wildcard $(SRC_DIR)/kernel/*.c) \
		  $(wildcard $(SRC_DIR)/drivers/*.c) \
			$(wildcard $(SRC_DIR)/lib/*.c)

BOOTLOADER_BIN := $(BIN_DIR)/bootloader.bin
KERNEL_BIN := $(BIN_DIR)/kernel.bin
IMG := $(OUT_DIR)/ssos.img

KERNEL_OBJS := $(patsubst $(SRC_DIR)/%, $(OBJS_DIR)/%.o, $(KERNEL_SOURCES))
BOOTLOADER_OBJS := $(patsubst $(SRC_DIR)/%, $(OBJS_DIR)/%.o, $(BOOTLOADER_SOURCES))

.PHONY: all clean

all: $(IMG)

$(BOOTLOADER_BIN): $(BOOTLOADER_SOURCES)
	@mkdir -p $(BIN_DIR)
	$(AS) $(AS_BIN_FLAGS) -o $@ $(SRC_DIR)/bootloader/start.S

$(OBJS_DIR)/%.S.o: $(SRC_DIR)/%.S
	@mkdir -p $(dir $@)
	$(AS) $(AS_ELF_FLAGS) -o $@ $<

$(OBJS_DIR)/%.c.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $<

$(KERNEL_BIN): $(KERNEL_OBJS)
	@mkdir -p $(BIN_DIR)
	$(LD) $(LDFLAGS) -o $@ $^

$(IMG): $(BOOTLOADER_BIN) $(KERNEL_BIN)
	@mkdir -p $(OUT_DIR)
	cat $^ > $@

clean:
	rm -rf $(OUT_DIR)/*
