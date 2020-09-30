rm -rf build 2> /dev/null
rm -rf isobuild 2> /dev/null
rm myos.iso 2> /dev/null


# Unzip large files
ls -l ./linux-i686-elf/libexec/gcc/i686-elf/*.tar.gz 2> /dev/null && (
    cd ./linux-i686-elf/libexec/gcc/i686-elf/
    tar -xvf cc1.tar.gz
    tar -xvf cc1plus.tar.gz
    tar -xvf lto1.tar.gz
    rm -f *.tar.gz
    cd ../../../..
)


# Build kernel
mkdir build

( # Try
    linux-i686-elf/bin/i686-elf-g++ -c kernel.c++ -o build/kernel.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
    linux-i686-elf/bin/i686-elf-as boot.s -o build/boot.o
    linux-i686-elf/bin/i686-elf-g++ -T linker.ld -o build/myos.bin -ffreestanding -O2 -nostdlib build/boot.o build/kernel.o -lgcc
) || ( # Catch
    echo 'Build failed!'
    exit 1
)


# Package kernel into a bootable ISO with grub
mkdir -p isobuild/boot/grub
cp build/myos.bin isobuild/boot/myos.bin
cp grub.cfg isobuild/boot/grub/grub.cfg

( # Try
    grub-file --is-x86-multiboot build/myos.bin && echo 'Kernel bootable with GRUB!'
    grub-mkrescue -o build/myos.iso isobuild 2> /dev/null && echo 'ISO ready!'
) || ( # Catch
    echo 'Not bootable with GRUB!'
    exit 1
)


# Launch with qemu
qemu-system-i386 -cdrom build/myos.iso
