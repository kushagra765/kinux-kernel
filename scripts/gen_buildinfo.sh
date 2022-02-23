#!/bin/sh

VERSION="0.01"
GCCVERSION=`./toolchain/compiler/bin/i686-elf-gcc --version | grep ^i686-elf-gcc | sed 's/^.* //g'`

mkdir -p include/generated/

echo "#ifndef _KINUX_BUILD_INFO_H\n#define _KINUX_BUILD_INFO_H\n\n#define BUILD_INFO \"Kinux version $VERSION (`whoami`@`hostname`) i686-elf-gcc ($GCCVERSION)\"\n\n#endif //_KINUX_BUILD_INFO_H" > include/generated/buildinfo.h
