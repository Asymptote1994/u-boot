make distclean
make jz2440_defconfig
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- -j8
