# Basic OS

This OS is written in C++ with a tiny bit of assembly!
You can build your own bootable ISO by running a script!

## Build with Windows

### Setup

  1. Install [Qemu for Windows](https://qemu.weilnetz.de/w64/)
  2. Install Windows Subsystem for Linux ([How to set up WSL](https://youtu.be/Cvrqmq9A3tA?t=107))
  3. Use this command to install GRUB with the ability to automatically generate a hybrid ISO (supporting both legacy BIOS and EFI) inside of your WSL shell:

```bash
# Depenencies for building a bootable hybrid ISO file using WSL
sudo apt-get install xorriso grub-legacy grub-pc-bin
```

### Build and Run

Then run `./build.ps1` and boot up with the ISO file in `build/myos.iso` using VirtualBox or Qemu!
