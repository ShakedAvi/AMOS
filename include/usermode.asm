global jump_usermode
extern test_user_function
extern syscall_print

jump_usermode:
   mov ax, 0x23
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax

   push 0x23
   push esp
   pushfd
   push 0x1B
   lea eax, [a]
   push eax

   iretd
a:
   ;mov eax, 0
   int 0x80
   jmp $
