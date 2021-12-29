CC = i686-elf-gcc
LD = $(CC)
AS = nasm
KERNEL = kinux-kernel.elf
ISO = kinux-kernel.iso

INCLUDE = -Iinclude/ -Idrivers/
CFLAGS = $(INCLUDE) -ffreestanding -Wno-discarded-qualifiers
ASFLAGS = -f elf32
LDFLAGS = -T link.ld -ffreestanding -O2 -nostdlib

CFILES = $(shell find drivers/ init/ kernel/ lib/ mm/ -name '*.c')
ASMFILES = $(shell find boot/ kernel/ -name '*.asm')
OBJFILES = $(CFILES:.c=.o) $(ASMFILES:.asm=.o)

$(ISO): $(KERNEL)
	@mkdir -p isodir/boot/grub
	@cp grub.cfg isodir/boot/grub/
	@cp $(KERNEL) isodir/boot/
	@grub-mkrescue -o $(ISO) isodir

$(KERNEL): boot/boot.o $(OBJFILES)
	@echo "LD $@"
	@$(LD) $(LDFLAGS) $^ -o $@

%.o: %.c
	@echo "CC $<"
	@$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	@echo "AS $<"
	@$(AS) $(ASFLAGS) $< -o $@
	
run:
	@qemu-system-x86_64 $(ISO)

clean:
	@rm -rf isodir $(ISO) $(KERNEL) $(OBJFILES)
