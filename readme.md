# Basic OS

This OS is written in C++ with a tiny bit of assembly!

You can build a bootable ISO by running a script!

## Build with Windows

  1. Install [Qemu for Windows](https://qemu.weilnetz.de/w64/)
  2. Install Windows Subsystem for Linux ([How to set up WSL](https://youtu.be/Cvrqmq9A3tA?t=107))
  3. Use this command to install GRUB with the ability to automatically generate a hybrid ISO (supporting both legacy BIOS and EFI) inside of your WSL shell:

```bash
# Depenencies for building a bootable hybrid ISO file using WSL
sudo apt-get install -y xorriso grub-legacy grub-pc-bin
```

Then run `./build.ps1` and boot up with the ISO file in `build/myos.iso` using VirtualBox or Qemu!

## Build with Linux

> Don't use Windows Subsystem for Linux here - it has known bugs and won't work!
>
> Only tested on Debian, should work on all Debian-based systems, such as Ubuntu.

Install all needed dependencies with these commands:

```bash
# Dependencies for compiling kernel
sudo apt-get install -y build-essential binutils bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo libisl-dev

# Depenencies for building a bootable hybrid ISO file using WSL
sudo apt-get install -y xorriso grub-legacy grub-pc-bin

# For qemu, to run the image
sudo apt-get install -y qemu-system-i386
```

Then run `./build.sh` and boot up with the ISO file in `build/myos.iso` using VirtualBox or Qemu!
