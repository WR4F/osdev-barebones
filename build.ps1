cls
echo "############# STARTING BUILD... #############"
Set-PSDebug -Trace 0
$ErrorActionPreference='Stop'
rm -Force -Recurse build
rm -Force -Recurse isobuild


# Build kernel
mkdir build
win-i686-elf\bin\i686-elf-gpp.exe -c kernel.cpp -o build\kernel.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
# win-i686-elf\bin\i686-elf-gpp.exe -c example_programs\calculator.cpp -o build\calculator.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
win-i686-elf\bin\i686-elf-as.exe boot.s -o build\boot.o
# win-i686-elf\bin\i686-elf-gpp.exe -T linker.ld -o build\myos.bin -ffreestanding -O2 -nostdlib build\boot.o build\calculator.o build\kernel.o -lgcc
win-i686-elf\bin\i686-elf-gpp.exe -T linker.ld -o build\myos.bin -ffreestanding -O2 -nostdlib build\boot.o build\kernel.o -lgcc


# Package kernel into a bootable binary
mkdir isobuild\boot\grub
cp build\myos.bin isobuild\boot\myos.bin
cp grub.cfg isobuild\boot\grub\grub.cfg

# Build ISO from binary, package with GRUB
bash -c "grub-file --is-x86-multiboot build/myos.bin && echo 'Kernel bootable with GRUB!'"
bash -c "grub-mkrescue -o build/myos.iso isobuild 2> /dev/null && echo 'ISO ready!'"
# Launch with qemu
qemu-system-i386 -cdrom build\myos.iso

# For faster testing, use this instead of building ISO
# qemu-system-i386 -kernel build\myos.bin
