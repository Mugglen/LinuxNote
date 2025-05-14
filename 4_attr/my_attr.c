#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/slab.h>

/*
 * Demo演示通过__ATTR宏创建sysfs属性文件
 * __ATTR宏用于简化sysfs属性定义，封装了属性的创建参数
 */

// 定义一个设备结构体来模拟真实设备
struct my_device {
    struct kobject kobj;      // 内嵌的kobject
    int value;                // 设备属性值
    char name[32];            // 设备名称
};

// 全局设备对象
static struct my_device *my_dev;

/**
 * value_show - value属性的读取函数
 * @kobj: 与此属性关联的kobject指针，通过container_of可获取父结构体
 * @attr: 属性结构体指针，包含了当前被读取的属性信息
 * @buf: 输出缓冲区，函数需要将要显示的数据写入此缓冲区
 *
 * 返回值: 写入缓冲区的字节数
 */
static ssize_t value_show(struct kobject *kobj, struct kobj_attribute *attr,
                        char *buf)
{
    struct my_device *dev = container_of(kobj, struct my_device, kobj);
    return sprintf(buf, "%d\n", dev->value);
}

/**
 * value_store - value属性的写入函数
 * @kobj: 与此属性关联的kobject指针，通过container_of可获取父结构体
 * @attr: 属性结构体指针，包含了当前被写入的属性信息
 * @buf: 输入缓冲区，包含用户写入的数据
 * @count: 输入缓冲区中的字节数
 *
 * 返回值: 成功时返回写入的字节数，失败时返回负的错误码
 */
static ssize_t value_store(struct kobject *kobj, struct kobj_attribute *attr,
                        const char *buf, size_t count)
{
    struct my_device *dev = container_of(kobj, struct my_device, kobj);
    int ret;
    
    ret = kstrtoint(buf, 10, &dev->value);
    if (ret < 0)
        return ret;
    
    pr_info("Value set to: %d\n", dev->value);
    return count;
}

/**
 * name_show - name属性的读取函数
 * @kobj: 与此属性关联的kobject指针，通过container_of可获取父结构体
 * @attr: 属性结构体指针，包含了当前被读取的属性信息
 * @buf: 输出缓冲区，函数需要将要显示的数据写入此缓冲区
 *
 * 返回值: 写入缓冲区的字节数
 */
static ssize_t name_show(struct kobject *kobj, struct kobj_attribute *attr,
                      char *buf)
{
    struct my_device *dev = container_of(kobj, struct my_device, kobj);
    return sprintf(buf, "%s\n", dev->name);
}

/**
 * name_store - name属性的写入函数
 * @kobj: 与此属性关联的kobject指针，通过container_of可获取父结构体
 * @attr: 属性结构体指针，包含了当前被写入的属性信息
 * @buf: 输入缓冲区，包含用户写入的数据
 * @count: 输入缓冲区中的字节数
 *
 * 返回值: 成功时返回写入的字节数，失败时返回负的错误码
 */
static ssize_t name_store(struct kobject *kobj, struct kobj_attribute *attr,
                       const char *buf, size_t count)
{
    struct my_device *dev = container_of(kobj, struct my_device, kobj);
    
    // 复制新名称，确保不超过缓冲区大小
    strncpy(dev->name, buf, sizeof(dev->name) - 1);
    
    // 去除可能的换行符
    if (dev->name[count-1] == '\n')
        dev->name[count-1] = '\0';
    else
        dev->name[count] = '\0';
    
    pr_info("Name set to: %s\n", dev->name);
    return count;
}

// 使用__ATTR宏定义属性
// __ATTR(_name, _mode, _show, _store) 对应参数为：
// 属性名, 权限模式, 读函数, 写函数
static struct kobj_attribute value_attribute = 
    __ATTR(value, 0664, value_show, value_store);

static struct kobj_attribute name_attribute = 
    __ATTR(name, 0664, name_show, name_store);

// 属性数组，列出所有要创建的属性
static struct attribute *my_attrs[] = {
    &value_attribute.attr,
    &name_attribute.attr,
    NULL,  // 以NULL结尾
};

// 属性组，方便管理多个属性
static struct attribute_group my_attr_group = {
    .attrs = my_attrs,
};

// 设备释放函数
static void my_device_release(struct kobject *kobj)
{
    struct my_device *dev = container_of(kobj, struct my_device, kobj);
    pr_info("Releasing device: %s\n", dev->name);
    kfree(dev);
}

// 定义kobject的操作
static struct kobj_type my_ktype = {
    .release = my_device_release,
    .sysfs_ops = &kobj_sysfs_ops,  // 使用默认的sysfs操作
};

static int __init my_attr_init(void)
{
    int ret;
    
    pr_info("Attribute demo module initializing\n");
    
    // 分配设备内存
    my_dev = kzalloc(sizeof(*my_dev), GFP_KERNEL);
    if (!my_dev)
        return -ENOMEM;
    
    // 初始化设备值
    my_dev->value = 100;
    strcpy(my_dev->name, "my_device");
    
    // 初始化kobject并注册
    ret = kobject_init_and_add(&my_dev->kobj, &my_ktype, NULL, "my_attr_demo");
    if (ret) {
        pr_err("Failed to initialize and add kobject\n");
        kfree(my_dev);
        return ret;
    }
    
    // 创建属性文件
    ret = sysfs_create_group(&my_dev->kobj, &my_attr_group);
    if (ret) {
        pr_err("Failed to create sysfs group\n");
        kobject_put(&my_dev->kobj);
        return ret;
    }
    
    pr_info("Attribute files created in /sys/my_attr_demo/\n");
    return 0;
}

static void __exit my_attr_exit(void)
{
    pr_info("Attribute demo module exiting\n");
    
    if (my_dev) {
        // 移除属性组
        sysfs_remove_group(&my_dev->kobj, &my_attr_group);
        // 释放kobject（会调用release函数）
        kobject_put(&my_dev->kobj);
    }
    
    pr_info("Attribute demo module cleaned up\n");
}

module_init(my_attr_init);
module_exit(my_attr_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("SYSFS Attribute Demo using __ATTR macro"); 
