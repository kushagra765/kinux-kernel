# Kinux Kernel
A kernel for x86 based systems.

## Building and running
Before building the kernel, you need to build the `i686-elf-gcc` toolchain. You can build it by running:
```
cd ./toolchain/
sh ./build_toolchain.sh
```
After the toolchain is built, run `make all` to build the kernel. After the kernel is built, you can run the kernel by running `make run`.

## TODO
- [x] Console
- [x] Interrupts
- [x] Keyboard driver
- [x] PMM and VMM
- [x] Initrd
- [ ] VFS
- [ ] Multitasking


## License
This project is licensed under GPL v2. See the [`LICENSE`](LICENSE) file for more info.
