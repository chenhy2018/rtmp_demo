
export PROJECT_PATH=${shell pwd}
export CROSS_COMPILE=arm-linux-gnueabihf-


all:
	make -C src

clean:
	make clean -C src

distclean:
	make distclean -C src

menuconfig:
	./conf/mconf ./conf/qn_config
