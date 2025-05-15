#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/fs.h>      /* 为设备号函数添加 */

/*
 * 设备注册示例
 * 演示如何在自定义总线(my_bus)上注册设备
 */

/* 声明来自my_bus.ko模块的外部总线类型 */
extern struct bus_type my_bus_type;

/* 设备号变量 */
static dev_t my_dev_number;  /* 用于存储动态分配的设备号 */

/* 定义设备的私有数据结构 */
struct my_device_data {
    char name[32];
    int id;
};

/* 静态定义设备数据 */
static struct my_device_data device_data = {
    .name = "示例设备",
    .id = 123
};

/* 释放设备结构体的回调函数 */
static void my_device_release(struct device *dev)
{
    pr_info("释放设备: %s\n", dev_name(dev));
    /* 注意：使用静态设备数据，不需要释放内存 */
}

/* 静态定义设备结构体 */
static struct device my_device = {
    .init_name = "my_device",     /* 设备名称 */
    .release = my_device_release, /* 释放回调 */
};

/* 初始化函数 */
static int __init my_device_init(void)
{
    int ret;

    pr_info("注册设备到my_bus总线...\n");

    /* 1. 动态申请设备号 - 从0开始分配1个设备号 */
    ret = alloc_chrdev_region(&my_dev_number, 0, 1, "my_device");
    if (ret < 0) {
        pr_err("无法分配设备号: %d\n", ret);
        return ret;
    }
    
    pr_info("成功分配设备号: 主设备号=%d, 次设备号=%d\n", 
            MAJOR(my_dev_number), MINOR(my_dev_number));
    
    /* 2. 设置设备号 */
    my_device.devt = my_dev_number;
    
    /* 3. 将数据与设备关联 */
    dev_set_drvdata(&my_device, &device_data);
    
    /* 4. 将设备与总线关联 */
    my_device.bus = &my_bus_type;
    
    /* 5. 注册设备 */
    ret = device_register(&my_device);
    if (ret) {
        pr_err("设备注册失败: %d\n", ret);
        unregister_chrdev_region(my_dev_number, 1);  /* 释放已分配的设备号 */
        return ret;
    }
    
    pr_info("设备注册成功 (主设备号: %d, 次设备号: %d)\n", 
           MAJOR(my_device.devt), MINOR(my_device.devt));
    return 0;
}

/* 退出函数 */
static void __exit my_device_exit(void)
{
    pr_info("注销设备...\n");
    
    /* 1. 注销设备 */
    device_unregister(&my_device);
    
    /* 2. 释放设备号 */
    unregister_chrdev_region(my_dev_number, 1);
    
    pr_info("设备注销完成\n");
}

module_init(my_device_init);
module_exit(my_device_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Device Registration Demo for my_bus"); 