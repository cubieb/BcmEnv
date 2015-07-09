PATH:=/opt/i686-pc-linux-gnu/bin:$(PATH)

.PHONY: all tools configure build install clean distclean
all: build

configure:
	rm -rf cfe-ntsw-5.1.4 cfe.bin
	unzip cfe-ntsw-5.1.4.zip
	#cd cfe-ntsw-5.1.4; patch -p1 < ../patches/001.bcm953000_devs.c.patch || exit 1;	

Env = CFLAGS="-DCFG_FULLDIAG -DCFG_SERIAL_BAUD_RATE=115200" TOOLPREFIX=sb1-elf- 
build:
	env $(Env) make -C cfe-ntsw-5.1.4/build/broadcom/bcm953003rsp 

install:
	cp cfe-ntsw-5.1.4/build/broadcom/bcm953003rsp/be/cfe.bin $(TftpRoot)

clean:
	make -C cfe-ntsw-5.1.4/build/broadcom/bcm953003rsp clean

distclean:
	rm -f $(TftpRoot)/cfe.bin
	rm -rf cfe-ntsw-5.1.4
