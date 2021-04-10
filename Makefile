CC = ~/Documents/Cross/Install/bin/i686-elf-gcc
CFLAGS = -ffreestanding -m32 -c

ASM = nasm
ASM_FLAGS = -f elf32

LD = ~/Documents/Cross/Install/bin/i686-elf-ld
LD_FLAGS = -m elf_i386 -T link.ld -o

QEMU = qemu-system-x86_64
QEMU_FLAGS = -soundhw pcspk -machine q35 -usb -usbdevice keyboard -drive file=disk.img -cdrom

build:
	mkdir -p obj

	$(ASM) $(ASM_FLAGS) kasm.asm -o kasm.o
	$(ASM) $(ASM_FLAGS) include/isr.asm -o obj/isrAsm.o
	$(ASM) $(ASM_FLAGS) include/gdt.asm -o obj/gdtAsm.o
	$(ASM) $(ASM_FLAGS) include/switch.asm -o obj/switch.o
	$(ASM) $(ASM_FLAGS) include/usermode.asm -o obj/usermode.o
	$(ASM) $(ASM_FLAGS) include/paging.asm -o obj/pagingAsm.o
	$(ASM) $(ASM_FLAGS) include/graphic.asm -o obj/graphicAsm.o

	$(CC) kernel.c -o kc.o $(CFLAGS)
	$(CC) include/system.c -o obj/system.o $(CFLAGS)
	$(CC) include/isr.c -o obj/isr.o $(CFLAGS)
	$(CC) include/idt.c -o obj/idt.o $(CFLAGS)
	$(CC) include/util.c -o obj/util.o $(CFLAGS)
	$(CC) include/string.c -o obj/string.o $(CFLAGS)
	$(CC) include/screen.c -o obj/screen.o $(CFLAGS)
	$(CC) include/keyboard.c -o obj/keyboard.o $(CFLAGS)
	$(CC) include/paging.c -o obj/paging.o $(CFLAGS)
	$(CC) include/timer.c -o obj/timer.o $(CFLAGS)
	$(CC) include/kheap.c -o obj/kheap.o $(CFLAGS)
	$(CC) include/orderedArray.c -o obj/orderedArray.o $(CFLAGS)
	$(CC) include/task.c -o obj/task.o $(CFLAGS)
	$(CC) include/gdt.c -o obj/gdt.o $(CFLAGS)
	$(CC) include/chess.c -o obj/chess.o $(CFLAGS)
	$(CC) include/procFS.c -o obj/procFS.o $(CFLAGS)
	$(CC) include/syscall.c -o obj/syscall.o $(CFLAGS)
	$(CC) include/mouse.c -o obj/mouse.o $(CFLAGS)
	$(CC) include/sound.c -o obj/sound.o $(CFLAGS)
	$(CC) include/stack.c -o obj/stack.o $(CFLAGS)
	$(CC) include/interpreter.c -o obj/interpreter.o $(CFLAGS)
	$(CC) include/pci.c -o obj/pci.o $(CFLAGS)
	$(CC) include/ahci.c -o obj/ahci.o $(CFLAGS)
	$(CC) include/disk.c -o obj/disk.o $(CFLAGS)
	$(CC) include/tar.c -o obj/tar.o $(CFLAGS)
	$(CC) include/graphic.c -o obj/graphic.o $(CFLAGS)
	$(CC) include/rtc.c -o obj/rtc.o $(CFLAGS)
	$(CC) include/uhci.c -o obj/uhci.o $(CFLAGS)

	$(LD) $(LD_FLAGS) AMOS/boot/AMOS.bin kasm.o kc.o obj/isr.o obj/string.o obj/system.o obj/screen.o obj/keyboard.o obj/util.o obj/idt.o obj/paging.o obj/kheap.o obj/timer.o obj/isrAsm.o obj/orderedArray.o obj/task.o obj/gdt.o obj/gdtAsm.o obj/usermode.o obj/pagingAsm.o obj/chess.o obj/procFS.o obj/syscall.o obj/switch.o obj/mouse.o obj/sound.o obj/stack.o obj/interpreter.o obj/pci.o obj/ahci.o obj/disk.o obj/tar.o obj/graphicAsm.o obj/graphic.o obj/rtc.o obj/uhci.o

clear:
	rm -f AMOS/boot/AMOS.bin
	rm -f kc.o
	rm -f kasm.o
	rm -r -f obj
	rm AMOS.iso

run:
	make
	mkdir -p AMOS/boot/grub
	grub-mkrescue -o AMOS.iso AMOS
	$(QEMU) $(QEMU_FLAGS) AMOS.iso
	#grub-mkrescue -o AMOS.iso AMOS/
