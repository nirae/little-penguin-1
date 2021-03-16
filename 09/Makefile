obj-m += mymounts.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

info:
	modinfo mymounts.ko

insert:
	insmod mymounts.ko

remove:
	rmmod mymounts.ko
