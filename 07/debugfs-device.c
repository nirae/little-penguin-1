#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/jiffies.h>
#include <linux/string.h>
#include <linux/mutex.h>

MODULE_AUTHOR("ndubouil");
MODULE_DESCRIPTION("debugfs device");
MODULE_LICENSE("GPL");

#define LOGIN "ndubouil"

static struct dentry *fortytwo = 0;
static DEFINE_MUTEX(lock);
static char foo_data[PAGE_SIZE];

static ssize_t id_read(struct file *filp, char __user *to, size_t count,
                        loff_t *ppos)
{
    /* https://elixir.bootlin.com/linux/latest/source/fs/libfs.c#L717 */
    return simple_read_from_buffer(to, count, ppos, LOGIN, sizeof(LOGIN));
}

static ssize_t id_write(struct file *filp, const char __user *from,
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

static ssize_t jiffies_read(struct file *filp, char __user *to, size_t count,
                        loff_t *ppos)
{
    /* https://elixir.bootlin.com/linux/latest/source/fs/libfs.c#L717 */

    char result[11];

    memset(result, 0, sizeof(result));

    snprintf(result, 11, "%ld", jiffies);

    return simple_read_from_buffer(to, count, ppos, result, sizeof(result));
}

static ssize_t foo_read(struct file *filp, char __user *to, size_t count,
                        loff_t *ppos)
{
    int ret = 0;
    /* https://elixir.bootlin.com/linux/latest/source/fs/libfs.c#L717 */
    ret = mutex_lock_interruptible(&lock);
    if (ret != 0)
        return ret;
    ret = simple_read_from_buffer(to, count, ppos, foo_data, strlen(foo_data));
    mutex_unlock(&lock);
    return ret;
}

static ssize_t foo_write(struct file *filp, const char __user *from,
                            size_t count, loff_t *ppos)
{
    ssize_t ret;

    if (count >= sizeof(foo_data))
        return -EINVAL;
    ret = mutex_lock_interruptible(&lock);
    if (ret != 0)
        return ret;
    memset(foo_data, 0, sizeof(foo_data));
    /* https://elixir.bootlin.com/linux/latest/source/fs/libfs.c#L752 */
    ret = simple_write_to_buffer(foo_data, count, ppos, from, count);
    mutex_unlock(&lock);
    if (ret < 0)
        return ret;
    return count;
}

struct file_operations id_fops = {
    .owner = THIS_MODULE,
    .read = id_read,
    .write = id_write
};

struct file_operations jiffies_fops = {
    .owner = THIS_MODULE,
    .read = jiffies_read,
};

struct file_operations foo_fops = {
    .owner = THIS_MODULE,
    .read = foo_read,
    .write = foo_write
};

int __init debugfs_init(void)
{
    mutex_init(&lock);
    fortytwo = debugfs_create_dir("fortytwo", NULL);
    if (!fortytwo)
    {
		printk(KERN_ERR "CONFIG_DEBUG_FS not set\n");
		return -ENODEV;
	}

    debugfs_create_file("id", 0666, fortytwo, NULL, &id_fops);
    debugfs_create_file("jiffies", 0444, fortytwo, NULL, &jiffies_fops);
    debugfs_create_file("foo", 0644, fortytwo, NULL, &foo_fops);

    return 0;
}

void __exit debugfs_exit(void)
{
    debugfs_remove_recursive(fortytwo);
}

module_init(debugfs_init);
module_exit(debugfs_exit);
