obj-m += hello-world.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

info:
	modinfo hello-world.ko

insert:
	insmod hello-world.ko

remove:
	rmmod hello-world.ko || true
	rmmod hello-world || true
