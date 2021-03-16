clone:
	git clone git://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git linux-next
	cd linux-next && \
	git remote add linux-next git://git.kernel.org/pub/scm/linux/kernel/git/next/linux-next.git && \
	git fetch linux-next && \
	git fetch --tags linux-next && \

log:
	journalctl -b
