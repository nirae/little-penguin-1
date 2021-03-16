obj-m += hello-world-usb.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

info:
	modinfo hello-world-usb.ko

insert:
	insmod hello-world-usb.ko

remove:
	rmmod hello-world.ko || true
	rmmod hello-world || true

udev-rule: remove
	cp usb-hello.rules /etc/udev/rules.d/
	udevadm control --reload
	lsmod
