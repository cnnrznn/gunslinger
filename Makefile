obj-m += mgunslinger.o
mgunslinger-objs += src/gunslinger.o
mgunslinger-objs += src/gs-interface.o
mgunslinger-objs += src/gs-log.o
mgunslinger-objs += src/gs-mm.o
#mgunslinger-objs += src/rlist.o

ccflags-y := -I$(src)/include

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
