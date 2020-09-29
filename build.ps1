$ErrorActionPreference='silentlycontinue'
rm -Force -Recurse build
rm -Force -Recurse isobuild
rm myos.iso


# Build kernel
mkdir build

try {
    win-i686-elf\bin\i686-elf-g++.exe -c kernel.c++ -o build\kernel.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
    win-i686-elf\bin\i686-elf-as.exe boot.s -o build\boot.o
    win-i686-elf\bin\i686-elf-g++.exe -T linker.ld -o build\myos.bin -ffreestanding -O2 -nostdlib build\boot.o build\kernel.o -lgcc
}
catch {
    echo 'Build failed!'
    exit 1
}


# Package kernel into a bootable ISO with grub
mkdir isobuild\boot\grub
cp build\myos.bin isobuild\boot\myos.bin
cp grub.cfg isobuild\boot\grub\grub.cfg
try {
    bash -c "grub-file --is-x86-multiboot build/myos.bin && echo 'Kernel bootable with GRUB!'"
    bash -c "grub-mkrescue -o build/myos.iso isobuild 2> /dev/null && echo 'ISO ready!'"
}
catch {
    echo 'Not bootable with GRUB!'
    exit 1
}


# Launch with qemu
qemu-system-i386 -cdrom build\myos.iso
