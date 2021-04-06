global jump_usermode

jump_usermode:
   mov ax, 0x23              ; ring 3 data
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax                ; SS is handled by iret

   push 0x23                 ; data selector
   push esp                  ; pushing current esp
   pushfd                    ; eflags
   push 0x1B                 ; code selector
   lea eax, [test_user_mode] ; instruction address to return to
   push eax

   iretd
test_user_mode:
   int 0x80     ; testing a syscall
   jmp $
