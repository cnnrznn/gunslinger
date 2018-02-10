obj-m += gunslinger.o
obj-m += gs-interface.o
obj-m += gs-log.o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
