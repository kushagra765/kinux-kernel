VERSION = 0.01
EXTRAVERSION =

ARCH = x86
CC = ./toolchain/compiler/bin/i686-elf-gcc
LD = $(CC)
AS = nasm
KERNEL = kinux-kernel.elf
INITRD = initrd.tar
ISO = kinux-kernel.iso

INCLUDE = -Iinclude/ -Idrivers/ -I$(PWD)
CFLAGS = $(INCLUDE) -ffreestanding -Wno-discarded-qualifiers -Wno-attributes
ASFLAGS = -f elf32
LDFLAGS = -T arch/$(ARCH)/link.ld -ffreestanding -O2 -nostdlib

CFILES = $(shell find drivers/ fs/ init/ kernel/ lib/ mm/ -name '*.c')
ASMFILES = $(shell find arch/$(ARCH)/ kernel/ -name '*.asm')
OBJFILES = $(CFILES:.c=.o) $(ASMFILES:.asm=.o)

$(ISO): $(KERNEL)
	@tar -zcvf $(INITRD) -C initrd/ .
	@mkdir -p isodir/boot/grub
	@cp grub.cfg isodir/boot/grub/
	@cp $(KERNEL) $(INITRD) isodir/boot/
	@grub-mkrescue -o $(ISO) isodir

$(KERNEL): arch/$(ARCH)/boot.o $(OBJFILES)
	@echo "LD $@"
	@$(LD) $(LDFLAGS) $^ -o $@

%.o: %.c
	@./scripts/gen_buildinfo.sh
	@echo "CC $<"
	@$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	@echo "AS $<"
	@$(AS) $(ASFLAGS) $< -o $@

run:
	@qemu-system-x86_64 -serial stdio $(ISO)

clean:
	@rm -rf isodir $(ISO) $(KERNEL) $(INITRD) $(OBJFILES) include/generated/
