# Basic OS

This OS is written in C++ with a tiny bit of assembly!

You can build a bootable ISO by running a script!

## Build with Windows

  1. Install [Qemu for Windows](https://qemu.weilnetz.de/w64/)
  2. Add `C:\Program Files\qemu` to your environment variables ([How-To Guide For Qemu](https://www.youtube.com/watch?v=al1cnTjeayk))
  3. Install Windows Subsystem for Linux ([How to set up WSL](https://youtu.be/Cvrqmq9A3tA?t=107))
  4. Use this command to install GRUB with the ability to automatically generate a hybrid ISO, supporting both legacy BIOS and EFI. **Run this inside of your WSL Bash shell**:

```bash
# Depenencies for building a bootable hybrid ISO file using WSL
sudo apt-get install -y grub || sudo apt-get install -y grub-pc
sudo apt-get install -y xorriso grub-pc-bin
```

Then, **in _Windows_**:
- Clone this repo (`git clone https://github.com/rslay/osdev-barebones`) or download the zip and unzip it somewhere
- In powershell, go to the `osdev-barebones` repo folder, and run the `./build.ps1` and boot up with the ISO file in `build/myos.iso` using VirtualBox or Qemu!


## Build with Linux

Sorry! Not yet supported.


<!--
> NOTE: Don't use Windows Subsystem for Linux here - it has known bugs and won't work!
>
> Only tested on Debian, should work on all Debian-based systems, such as Ubuntu.

Install all needed dependencies with these commands:

```bash
# Dependencies for compiling kernel
sudo apt-get update
sudo apt-get install -y build-essential binutils bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo libisl-dev lib32z1 lib32ncurses5 libc6

# Depenencies for building a bootable hybrid ISO file using WSL
sudo apt-get install -y xorriso grub-legacy grub-pc-bin

# For qemu, to run the image
sudo apt-get install -y qemu-system-i386
```

Then run `./build.sh` and boot up with the ISO file in `build/myos.iso` using VirtualBox or Qemu!
-->
