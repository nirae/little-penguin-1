#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>

MODULE_AUTHOR("ndubouil");
MODULE_DESCRIPTION("fortytwo misc char device driver");
MODULE_LICENSE("GPL");

#define LOGIN "ndubouil"

ssize_t fortytwo_misc_read(struct file *filp, char __user *to, size_t count,
                        loff_t *ppos)
{
    /* https://elixir.bootlin.com/linux/latest/source/fs/libfs.c#L717 */
    return simple_read_from_buffer(to, count, ppos, LOGIN, sizeof(LOGIN));
}

ssize_t fortytwo_misc_write(struct file *filp, const char __user *from,
                            size_t count, loff_t *ppos)
{
    char to[count];
    ssize_t retval;

    if (count != strlen(LOGIN))
        return -EINVAL;
    /* https://elixir.bootlin.com/linux/latest/source/fs/libfs.c#L752 */
    retval = simple_write_to_buffer(to, count, ppos, from, count);
    if (retval < 0)
        return retval;
    if (strncmp(LOGIN, to, count))
        return -EINVAL;
    return count;
}

struct file_operations misc_fops = {
    .owner = THIS_MODULE,
    .read = fortytwo_misc_read,
    .write = fortytwo_misc_write
};

struct miscdevice fortytwo_misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "fortytwo",
    .fops = &misc_fops
};

int misc_init(void)
{
    return misc_register(&fortytwo_misc);
}

void misc_exit(void)
{
    misc_deregister(&fortytwo_misc);
}

module_init(misc_init);
module_exit(misc_exit);
