build:
	mkdir -p obj

	nasm -f elf32 kasm.asm -o kasm.o
	nasm -f elf32 include/isr.asm -o obj/isrAsm.o
	nasm -f elf32 include/gdt.asm -o obj/gdtAsm.o
	nasm -f elf32 include/switch.asm -o obj/switch.o

	~/Documents/Cross/Install/bin/i686-elf-gcc -m32 -c kernel.c -o kc.o -ffreestanding
	~/Documents/Cross/Install/bin/i686-elf-gcc -m32 -c include/system.c -o obj/system.o -ffreestanding
	~/Documents/Cross/Install/bin/i686-elf-gcc -m32 -c include/isr.c -o obj/isr.o -ffreestanding
	~/Documents/Cross/Install/bin/i686-elf-gcc -m32 -c include/idt.c -o obj/idt.o -ffreestanding
	~/Documents/Cross/Install/bin/i686-elf-gcc -m32 -c include/util.c -o obj/util.o -ffreestanding
	~/Documents/Cross/Install/bin/i686-elf-gcc -m32 -c include/string.c -o obj/string.o -ffreestanding
	~/Documents/Cross/Install/bin/i686-elf-gcc -m32 -c include/screen.c -o obj/screen.o -ffreestanding
	~/Documents/Cross/Install/bin/i686-elf-gcc -m32 -c include/keyboard.c -o obj/keyboard.o -ffreestanding
	~/Documents/Cross/Install/bin/i686-elf-gcc -m32 -c include/paging.c -o obj/paging.o -ffreestanding
	~/Documents/Cross/Install/bin/i686-elf-gcc -m32 -c include/timer.c -o obj/timer.o -ffreestanding
	~/Documents/Cross/Install/bin/i686-elf-gcc -m32 -c include/kheap.c -o obj/kheap.o -ffreestanding
	~/Documents/Cross/Install/bin/i686-elf-gcc -m32 -c include/orderedArray.c -o obj/orderedArray.o -ffreestanding
	~/Documents/Cross/Install/bin/i686-elf-gcc -m32 -c include/task.c -o obj/task.o -ffreestanding
	~/Documents/Cross/Install/bin/i686-elf-gcc -m32 -c include/gdt.c -o obj/gdt.o -ffreestanding

	~/Documents/Cross/Install/bin/i686-elf-ld -m elf_i386 -T link.ld -o AMOS/boot/AMOS.bin kasm.o kc.o obj/isr.o obj/string.o obj/system.o obj/screen.o obj/keyboard.o obj/util.o obj/idt.o obj/paging.o obj/kheap.o obj/timer.o obj/isrAsm.o obj/orderedArray.o obj/switch.o obj/task.o obj/gdt.o obj/gdtAsm.o

clear:
	rm -f AMOS/boot/AMOS.bin
	rm -f kc.o
	rm -f kasm.o
	rm -r -f obj

run:
	make
	qemu-system-x86_64 -kernel AMOS/boot/AMOS.bin
	#grub-mkrescue -o AMOS.iso AMOS/
