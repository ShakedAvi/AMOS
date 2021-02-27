build:
	mkdir -p obj

	nasm -f elf32 kasm.asm -o kasm.o
	nasm -f elf32 include/isr.asm -o obj/isrAsm.o
	nasm -f elf32 include/gdt.asm -o obj/gdtAsm.o
	nasm -f elf32 include/switch.asm -o obj/switch.o
	nasm -f elf32 include/usermode.asm -o obj/usermodeAsm.o
	nasm -f elf32 include/paging.asm -o obj/pagingAsm.o
	nasm -f elf32 include/graphic.asm -o obj/graphicAsm.o

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
	~/Documents/Cross/Install/bin/i686-elf-gcc -m32 -c include/vfs.c -o obj/vfs.o -ffreestanding
	~/Documents/Cross/Install/bin/i686-elf-gcc -m32 -c include/usermode.c -o obj/usermode.o -ffreestanding
	~/Documents/Cross/Install/bin/i686-elf-gcc -m32 -c include/chess.c -o obj/chess.o -ffreestanding
	~/Documents/Cross/Install/bin/i686-elf-gcc -m32 -c include/procFS.c -o obj/procFS.o -ffreestanding
	~/Documents/Cross/Install/bin/i686-elf-gcc -m32 -c include/syscall.c -o obj/syscall.o -ffreestanding
	~/Documents/Cross/Install/bin/i686-elf-gcc -m32 -c include/mouse.c -o obj/mouse.o -ffreestanding
	~/Documents/Cross/Install/bin/i686-elf-gcc -m32 -c include/sound.c -o obj/sound.o -ffreestanding
	~/Documents/Cross/Install/bin/i686-elf-gcc -m32 -c include/stack.c -o obj/stack.o -ffreestanding
	~/Documents/Cross/Install/bin/i686-elf-gcc -m32 -c include/interpreter.c -o obj/interpreter.o -ffreestanding
	~/Documents/Cross/Install/bin/i686-elf-gcc -m32 -c include/pci.c -o obj/pci.o -ffreestanding
	~/Documents/Cross/Install/bin/i686-elf-gcc -m32 -c include/ahci.c -o obj/ahci.o -ffreestanding
	~/Documents/Cross/Install/bin/i686-elf-gcc -m32 -c include/disk.c -o obj/disk.o -ffreestanding
	~/Documents/Cross/Install/bin/i686-elf-gcc -m32 -c include/tar.c -o obj/tar.o -ffreestanding
	~/Documents/Cross/Install/bin/i686-elf-gcc -m32 -c include/graphic.c -o obj/graphic.o -ffreestanding
	~/Documents/Cross/Install/bin/i686-elf-gcc -m32 -c include/rtc.c -o obj/rtc.o -ffreestanding

	~/Documents/Cross/Install/bin/i686-elf-ld -m elf_i386 -T link.ld -o AMOS/boot/AMOS.bin kasm.o kc.o obj/isr.o obj/string.o obj/system.o obj/screen.o obj/keyboard.o obj/util.o obj/idt.o obj/paging.o obj/kheap.o obj/timer.o obj/isrAsm.o obj/orderedArray.o obj/task.o obj/gdt.o obj/gdtAsm.o obj/vfs.o obj/usermode.o obj/usermodeAsm.o obj/pagingAsm.o obj/chess.o obj/procFS.o obj/syscall.o obj/switch.o obj/mouse.o obj/sound.o obj/stack.o obj/interpreter.o obj/pci.o obj/ahci.o obj/disk.o obj/tar.o obj/graphicAsm.o obj/graphic.o obj/rtc.o

clear:
	rm -f AMOS/boot/AMOS.bin
	rm -f kc.o
	rm -f kasm.o
	rm -r -f obj
	rm AMOS.iso

run:
	make
	mkdir -p AMOS/boot/grub
	cp grub.cfg AMOS/boot/grub/grub.cfg
	grub-mkrescue -o AMOS.iso AMOS
	qemu-system-x86_64 -machine q35 -drive file=disk.img -cdrom AMOS.iso
	#grub-mkrescue -o AMOS.iso AMOS/
