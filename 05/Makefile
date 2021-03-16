obj-m += misc-char-device.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

info:
	modinfo misc-char-device.ko

insert:
	insmod misc-char-device.ko

remove:
	rmmod misc-char-device.ko
	rm -rf /dev/fortytwo
