#!/bin/bash



make clean
make
bootgen -w -image boot.bif -o boot.bin
cp boot.bin $1
sync
umount $1
