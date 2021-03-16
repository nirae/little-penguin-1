obj-m += file.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

info:
	modinfo file.ko

insert:
	insmod file.ko

remove:
	rmmod file.ko

style:
	/lib/modules/$(shell uname -r)/build/scripts/checkpatch.pl --file file.c || true

clang-format:
	clang-format -style=file file.c > file2.c
