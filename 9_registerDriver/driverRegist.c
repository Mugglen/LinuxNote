#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/fs.h>

/*
 * 驱动注册示例
 * 演示如何在自定义总线(my_bus)上注册驱动
 */

/* 声明来自my_bus.ko模块的外部总线类型 */
extern struct bus_type my_bus_type;

/* 定义驱动的私有数据结构 */
struct my_driver_data {
    char name[32];
    int id;
};

/* 静态定义驱动数据 - 使用全局变量 */
static struct my_driver_data driver_data = {
    .name = "示例驱动",
    .id = 123
};

/* 驱动探测函数 - 当找到匹配的设备时调用 */
static int my_driver_probe(struct device *dev)
{
    /* 直接使用全局变量driver_data */
    pr_info("驱动探测: 设备 %s\n", dev_name(dev));
    pr_info("驱动数据: name=%s, id=%d\n", driver_data.name, driver_data.id);
    
    /* 这里可以初始化设备，分配资源等 */
    return 0;
}

/* 驱动移除函数 - 当设备被移除时调用 */
static int my_driver_remove(struct device *dev)
{
    pr_info("驱动移除: 设备 %s\n", dev_name(dev));
    
    /* 这里可以清理资源，释放内存等 */
    return 0;
}

/* 定义驱动结构体 */
static struct device_driver my_driver = {
    /* 注意：根据my_bus_match的逻辑，驱动名必须与设备名完全相等才能匹配成功 */
    .name = "my_device",  /* 驱动名称 - 必须与设备名完全一致 */
    .bus = &my_bus_type,  /* 关联到my_bus总线 */
    .probe = my_driver_probe,    /* 探测函数 */
    .remove = my_driver_remove,  /* 移除函数 */
};

/* 初始化函数 */
static int __init my_driver_init(void)
{
    int ret;

    pr_info("注册驱动到my_bus总线...\n");
    
    /* 注册驱动 */
    ret = driver_register(&my_driver);
    if (ret) {
        pr_err("驱动注册失败: %d\n", ret);
        return ret;
    }
    
    pr_info("驱动注册成功\n");
    return 0;
}

/* 退出函数 */
static void __exit my_driver_exit(void)
{
    pr_info("注销驱动...\n");
    
    /* 注销驱动 */
    driver_unregister(&my_driver);
    
    pr_info("驱动注销完成\n");
}

module_init(my_driver_init);
module_exit(my_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Driver Registration Demo for my_bus"); 
