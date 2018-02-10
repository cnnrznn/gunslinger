obj-m += mgunslinger.o
mgunslinger-objs += gunslinger.o
mgunslinger-objs += gs-interface.o
mgunslinger-objs += gs-log.o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
