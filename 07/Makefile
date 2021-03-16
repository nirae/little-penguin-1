obj-m += debugfs-device.o

all:
	make -C /usr/src/kernel-5.10-debug M=$(PWD) modules

clean:
	make -C /usr/src/kernel-5.10-debug M=$(PWD) clean

info:
	modinfo debugfs-device.ko

insert:
	insmod debugfs-device.ko

remove:
	rmmod debugfs-device.ko

check_jiffies:
	cat /sys/kernel/debug/fortytwo/jiffies;echo;cat /proc/timer_list | grep 'jiffies:'

test_foo:
	./write_foo.py > /sys/kernel/debug/fortytwo/foo &
	cat /sys/kernel/debug/fortytwo/foo

clone:
	git clone git://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git linux-debug

boot-log:
	journalctl -b
