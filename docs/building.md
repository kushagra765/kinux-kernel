# Building the kernel

To build the kernel, you need the i686-elf-gcc toolchain and many other
packages. To install the packages and build the toolchain, see Building the
Toolchain. If you already have the toolchain, see Building without Toolchain.

NOTE: `./toolchain/build_toolchain.sh` uses apt to install packages. If you
use Arch or Fedora, comment out those lines and install them by yourself.
Also, if you use a Debian based distro, it is recommended that you copy the
`sudo apt install` lines on a terminal, install them and comment out the
apt install lines as they require sudo.

## Building the toolchain
Run these commands to build it:
```
cd ./toolchain/
chmod +x *.sh
./build_toolchain.sh
```
After the toolchain is built, cd back to the kinux-kernel directory and see
Building the kernel.

## Building without toolchain
Export $PATH to the location where your toolchain is installed. The path
should have `/bin` at the end of the location. Run `./scripts/install_deps.sh`
to install the required packages. Now, see Building the kernel.

NOTE: `./scripts/install_deps.sh` uses apt to install packages. If you use
Arch or Fedora, check the script and install them by yourself.

## Building the kernel
Run `make` to build the kernel. After the kernel is built, you can run it by
using `make run`.
