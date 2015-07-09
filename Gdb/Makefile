SourceDir=gdb-7.2
SourceTar=gdb-7.2a.tar.gz

.PHONY: all untar configure build install uninstall clean
all: build
	@

untar:
	$(RM) -r $(SourceDir)
	tar -xvf $(SourceTar)	

configure:
	#by default, "--prefix" is /usr/local.
	cd $(SourceDir)/gdb; ./configure

build:
	$(MAKE) -C $(SourceDir)/gdb

install:
	$(MAKE) -C $(SourceDir)/gdb install

uninstall:
	$(MAKE) -C $(SourceDir)/gdb uninstall

clean:
	$(RM) -r $(SourceDir)
