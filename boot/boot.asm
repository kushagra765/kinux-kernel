[BITS 32]

global start
start:
     mov esp, sys_stack
     jmp stublet
     
ALIGN 4
multiboot:
    PAGE_ALIGN   equ 1<<0
    MEM_INFO     equ 1<<1
    MAGIC        equ 0x1BADB002
    FLAGS        equ PAGE_ALIGN | MEM_INFO
    CHKSUM       equ -(MAGIC + FLAGS)
    
    EXTERN code, bss, end
    dd MAGIC
    dd FLAGS
    dd CHKSUM
    dd multiboot
    dd code
    dd bss
    dd end
    dd start
    
stublet:
     cli
     extern start_kernel
     push ebx
     call start_kernel
     jmp $
     
global gdt_flush
extern gdt_ptr
gdt_flush:
     lgdt [gdt_ptr]
     mov ax, 0x10
     mov ds, ax
     mov es, ax
     mov fs, ax
     mov gs, ax
     mov ss, ax
     jmp 0x08:flush
flush:
     ret
     
global load_idt
extern idt_ptr
load_idt:
     lidt [idt_ptr]
     ret
     
SECTION .bss
    resb 8192
sys_stack:
