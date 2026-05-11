CC := i686-elf-gcc
AS := i686-elf-as
LD := i686-elf-ld
QEMU := qemu-system-i386

CFLAGS := -O2 -g -ffreestanding -Wall -Wextra -m32 -masm=intel
LDFLAGS := -T linker.ld -melf_i386
ASMFLAGS := -mintel

# Directories
SRC_DIR := src
BUILD_DIR := build

# Source files
C_SRCS := $(wildcard $(SRC_DIR)/*.c)
ASM_SRCS := $(wildcard $(SRC_DIR)/*.s)

# Using a temporary variable as make doesn't allow self-referential definition
OBJECTS_TMP := $(C_SRCS:.c=.o)
OBJECTS_TMP += $(ASM_SRCS:.s=.o)
OBJECTS := $(subst $(SRC_DIR),$(BUILD_DIR),$(OBJECTS_TMP))

# Default target
all: os.iso

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile C files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@echo "--Building C $<--"
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble .s files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s | $(BUILD_DIR)
	@echo "--Building ASM $<--"
	nasm $< -f elf32 -o $@

# Boot.s is being assembled with GAS because it was too much of a pain converting it to Intel syntax
$(BUILD_DIR)/boot.o: boot.s
	i686-elf-as boot.s -o $(BUILD_DIR)/boot.o

# Link kernel
myos: $(OBJECTS) build/boot.o
	@echo "--Linking kernel--"
	$(LD) $(LDFLAGS) $^ -o $@

# Create ISO
os.iso: myos
	@echo "--Creating ISO--"
	mkdir -p iso/boot/grub
	cp myos isodir/boot/myos
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o os.iso isodir

# Run in QEMU
run: os.iso
	$(QEMU) -monitor stdio -cdrom $< -serial file:log.txt -audiodev pa,id=speaker -machine pcspk-audiodev=speaker

# Run in QEMU but also output when interrupts are being run to the terminal
run-output-interrupts: os.iso
	$(QEMU) -monitor stdio -cdrom $< -serial file:log.txt -d int -M smm=off -audiodev pa,id=speaker -machine pcspk-audiodev=speaker

# Run in QEMU, output interrupts, and start a GDB session on :1234
run-gdb: os.iso
	$(QEMU) -monitor stdio -cdrom $< -serial file:log.txt -d int -M smm=off -s -S -audiodev pa,id=speaker -machine pcspk-audiodev=speaker

# Clean
clean:
	rm -rf $(BUILD_DIR) iso os.iso kernel.elf

.PHONY: all run clean
