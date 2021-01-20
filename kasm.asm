MBOOT_PAGE_ALIGN    equ 1<<0    ; Load kernel and modules on a page boundary
MBOOT_MEM_INFO      equ 1<<1    ; Provide your kernel with memory info
MBOOT_HEADER_MAGIC  equ 0x1BADB002 ; Multiboot Magic value

MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

;stack: resb 0x4000

[BITS 32]

[GLOBAL mboot]
[EXTERN code]
[EXTERN bss]
[EXTERN end]

section .__mbHeader

mboot:
    dd  MBOOT_HEADER_MAGIC

    dd  MBOOT_HEADER_FLAGS
    dd  MBOOT_CHECKSUM

    dd  mboot
    dd  code
    dd  bss
    dd  end
    dd  start

section .text

[GLOBAL start]
[EXTERN kmain]

start:
    ;mov esp, stack
    push esp
    push ebx

    cli
    call kmain
    jmp $
