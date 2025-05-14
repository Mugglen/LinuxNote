#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/slab.h>

/*
 * Demo演示使用kobject_create_and_add自动创建kobject并注册属性
 * 这种方式更简单，无需手动处理kobject的生命周期
 */

// 全局kobject指针
static struct kobject *my_kobj;

// 全局变量，用于存储属性值
static int device_value = 100;
static char device_name[32] = "auto_device";

/**
 * value_show - value属性的读取函数
 * @kobj: 与此属性关联的kobject指针
 * @attr: 属性结构体指针，包含了当前被读取的属性信息
 * @buf: 输出缓冲区，函数需要将要显示的数据写入此缓冲区
 *
 * 返回值: 写入缓冲区的字节数
 */
static ssize_t value_show(struct kobject *kobj, struct kobj_attribute *attr,
                        char *buf)
{
    return sprintf(buf, "%d\n", device_value);
}

/**
 * value_store - value属性的写入函数
 * @kobj: 与此属性关联的kobject指针
 * @attr: 属性结构体指针，包含了当前被写入的属性信息
 * @buf: 输入缓冲区，包含用户写入的数据
 * @count: 输入缓冲区中的字节数
 *
 * 返回值: 成功时返回写入的字节数，失败时返回负的错误码
 */
static ssize_t value_store(struct kobject *kobj, struct kobj_attribute *attr,
                        const char *buf, size_t count)
{
    int ret;
    
    ret = kstrtoint(buf, 10, &device_value);
    if (ret < 0)
        return ret;
    
    pr_info("Value set to: %d\n", device_value);
    return count;
}

/**
 * name_show - name属性的读取函数
 * @kobj: 与此属性关联的kobject指针
 * @attr: 属性结构体指针，包含了当前被读取的属性信息
 * @buf: 输出缓冲区，函数需要将要显示的数据写入此缓冲区
 *
 * 返回值: 写入缓冲区的字节数
 */
static ssize_t name_show(struct kobject *kobj, struct kobj_attribute *attr,
                      char *buf)
{
    return sprintf(buf, "%s\n", device_name);
}

/**
 * name_store - name属性的写入函数
 * @kobj: 与此属性关联的kobject指针
 * @attr: 属性结构体指针，包含了当前被写入的属性信息
 * @buf: 输入缓冲区，包含用户写入的数据
 * @count: 输入缓冲区中的字节数
 *
 * 返回值: 成功时返回写入的字节数，失败时返回负的错误码
 */
static ssize_t name_store(struct kobject *kobj, struct kobj_attribute *attr,
                       const char *buf, size_t count)
{
    // 复制新名称，确保不超过缓冲区大小
    strncpy(device_name, buf, sizeof(device_name) - 1);
    
    // 去除可能的换行符
    if (device_name[count-1] == '\n')
        device_name[count-1] = '\0';
    else
        device_name[count] = '\0';
    
    pr_info("Name set to: %s\n", device_name);
    return count;
}

// 使用__ATTR宏定义属性
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

/**
 * 属性组，方便管理多个属性
 * 可以用以下几种方式定义:
 *
 * 1. 基本方式 (当前使用):
 * static struct attribute_group my_attr_group = {
 *     .attrs = my_attrs,
 * };
 *
 * 2. 带组名的方式:
 * static struct attribute_group my_attr_group = {
 *     .name = "my_group",  // 会在/sys/my_attr_auto/my_group/下创建属性文件
 *     .attrs = my_attrs,
 * };
 *
 * 3. 带自定义权限的方式:
 * static struct attribute_group my_attr_group = {
 *     .name = "my_group",
 *     .attrs = my_attrs,
 *     .bin_attrs = NULL,   // 可以包含二进制属性
 *     .is_visible = NULL,  // 可以设置动态显示/隐藏属性的回调函数
 * };
 */
static struct attribute_group my_attr_group = {
    .attrs = my_attrs,
    // .name = "my_group",  // 取消注释此行可在子目录下创建属性
};

static int __init my_attr_auto_init(void)
{
    int ret;
    
    pr_info("Auto attribute demo module initializing\n");
    
    /* 
     * kobject_create_and_add会自动创建一个kobject并添加到系统中
     * 参数1: kobject名称，将在sysfs中显示
     * 参数2: 父kobject，NULL表示在/sys/下创建
     */
    my_kobj = kobject_create_and_add("my_attr_auto", NULL);
    if (!my_kobj) {
        pr_err("Failed to create kobject\n");
        return -ENOMEM;
    }
    
    /* 
     * sysfs_create_group将属性组添加到kobject
     * 这将自动在/sys/my_attr_auto/下创建属性文件
     *
     * 与sysfs_create_file的区别:
     * 1. sysfs_create_group一次创建多个属性文件，而sysfs_create_file一次只创建一个
     * 2. 使用sysfs_create_group可以减少重复代码，特别是当有多个属性时
     * 3. 如果使用sysfs_create_file，需要为每个属性单独调用并处理错误:
     *    ret = sysfs_create_file(my_kobj, &value_attribute.attr);
     *    ret = sysfs_create_file(my_kobj, &name_attribute.attr);
     * 4. 使用group方式更易于维护，尤其是当属性数量较多时
     */
    ret = sysfs_create_group(my_kobj, &my_attr_group);
    if (ret) {
        pr_err("Failed to create sysfs group\n");
        kobject_put(my_kobj);  // 释放kobject
        return ret;
    }
    
    pr_info("Attribute files created in /sys/my_attr_auto/\n");
    return 0;
}

static void __exit my_attr_auto_exit(void)
{
    pr_info("Auto attribute demo module exiting\n");
    
    if (my_kobj) {
        // 移除属性组
        sysfs_remove_group(my_kobj, &my_attr_group);
        // 释放kobject
        kobject_put(my_kobj);
    }
    
    pr_info("Auto attribute demo module cleaned up\n");
}

module_init(my_attr_auto_init);
module_exit(my_attr_auto_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Auto SYSFS Attribute Demo using kobject_create_and_add"); 