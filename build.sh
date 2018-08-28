#!/bin/sh
make clean;make
arm-linux-gnueabihf-strip src/demo_rtmp
cp src/demo_rtmp /home/samba/Android_ajMount
