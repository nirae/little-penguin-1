#include <linux/module.h>
#include <linux/fs.h>
#include <linux/fs_struct.h>
#include <linux/init.h>
#include <linux/mount.h>
#include <linux/string.h>
#include <linux/proc_fs.h>
#include <../fs/mount.h>
#include <linux/nsproxy.h>
#include <linux/slab.h>

MODULE_AUTHOR("ndubouil");
MODULE_DESCRIPTION("list all mount points");
MODULE_LICENSE("GPL");

static struct proc_dir_entry *mymounts;


static int fill_with_parent_path(struct mount *mnt, char **buffer, int *i)
{
    char path[256];

    if (strcmp(mnt->mnt_parent->mnt_mountpoint->d_name.name, "/") != 0)
        fill_with_parent_path(mnt->mnt_parent, buffer, i);
    *i += snprintf(*buffer + *i, PAGE_SIZE - *i, "%s", dentry_path_raw(mnt->mnt_mountpoint, path, sizeof(path)));
    return *i;
}

static ssize_t mymounts_read(struct file *filp, char __user *to, size_t count,
                        loff_t *ppos)
{
    int ret = 0;
    int i = 0;
    char *buffer;
    char path[256];
    struct mount *mnt;

    buffer = kmalloc(PAGE_SIZE, GFP_KERNEL);
	if (!buffer)
		return -ENOMEM;

    /*
     *   Loop over all the mountpoints listed in the mount namespace current->nsproxy->mnt_ns
     *   (https://elixir.bootlin.com/linux/latest/source/include/linux/nsproxy.h)
     *   (https://stackoverflow.com/a/46609832)
     * 
     *   https://www.kernel.org/doc/htmldocs/kernel-api/API-list-for-each-entry.html
     */
    list_for_each_entry(mnt, &current->nsproxy->mnt_ns->list, mnt_list) {
        if (strcmp(mnt->mnt_devname, "none") == 0)
            continue;

        memset(path, 0, sizeof(path));

        /*
         *  First, retrieve and add the mountpoint name in the buffer
         *
         *      mnt->mnt_mountpoint->d_name.name
         * 
         *  There is 3 cases to manage to match the subject :
         *      - /dev/root : manually put "root"
         *      - / : need a specific padding for a pretty output
         *      - other cases
         */
        if (strcmp(mnt->mnt_devname, "/dev/root") == 0) {
            i += snprintf((&(buffer[i])), PAGE_SIZE - i, "%-20s", "root");
        }
        else if (strcmp(mnt->mnt_mountpoint->d_name.name, "/") == 0) {
            i += snprintf((&(buffer[i])), PAGE_SIZE - i, "%s%-*s", \
                mnt->mnt_parent->mnt_mountpoint->d_name.name, \
                (int)(20 - strlen(mnt->mnt_parent->mnt_mountpoint->d_name.name)), \
                mnt->mnt_mountpoint->d_name.name);
        }
        else {
            i += snprintf((&(buffer[i])), PAGE_SIZE - i, "%-20s", mnt->mnt_mountpoint->d_name.name);
        }

        /*
         *  Then, construct recursively the complete parent path of the mountpoint path target
         *   -> Reverse loop on mnt->mnt_parent
         */
        if (strcmp(mnt->mnt_parent->mnt_mountpoint->d_name.name, "/") != 0)
            fill_with_parent_path(mnt->mnt_parent, &buffer, &i);

        /*
         *  Finally add the filename of the mountpoint target to complete the path, and the line
         */
        i += snprintf((&(buffer[i])), PAGE_SIZE - i, "%s\n", \
            dentry_path_raw(mnt->mnt_mountpoint, path, sizeof(path)));
	}

    ret = simple_read_from_buffer(to, count, ppos, buffer, strlen(buffer));
    kfree(buffer);
    return ret;
}

struct proc_ops mymounts_ops = {
    .proc_read = mymounts_read,
};

int __init mymounts_init(void)
{
    /* https://elixir.bootlin.com/linux/latest/source/include/linux/proc_fs.h */
    mymounts = proc_create("mymounts", 0660, NULL, &mymounts_ops);
    return 0;
}

void __exit mymounts_exit(void)
{
    proc_remove(mymounts);
}

module_init(mymounts_init);
module_exit(mymounts_exit);
