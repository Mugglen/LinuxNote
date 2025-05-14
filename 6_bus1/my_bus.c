#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>

/*
 * 简单总线注册示例
 * 演示如何创建、注册和使用Linux内核中的总线类型
 */

/* 定义自定义设备ID结构体，用于设备与驱动匹配 */
struct my_device_id {
    char name[32];
    unsigned int type;
    unsigned long driver_data;
};

/* 宏定义，简化获取my_device_id的方法 */
#define to_my_id(drv) ((struct my_device_id *)drv->id_table)

/* 自定义匹配函数，当驱动程序和设备注册到总线时调用
 * 如果返回非零值，表示驱动和设备匹配，将调用驱动的probe函数
 * 
 * 实际驱动开发中常见的匹配机制：
 * 1. 通过设备ID表匹配 - 驱动程序提供一个ID表，列出所有支持的设备
 *    例如USB驱动中的 usb_device_id 或PCI驱动中的 pci_device_id
 *
 * 2. 特定设备属性匹配 - 检查设备特有的属性
 *    例如设备型号、版本号、兼容性字符串等
 *
 * 3. ACPI或设备树匹配 - 在支持ACPI或设备树的系统上
 *    通过"compatible"属性或ACPI ID进行匹配
 *
 * 4. 类型匹配 - 设备和驱动都声明支持特定类型
 *    例如网络设备类型、块设备类型等
 */
static int my_bus_match(struct device *dev, struct device_driver *drv)
{
    pr_info("匹配函数被调用: 设备=%s, 驱动=%s\n", dev->kobj.name, drv->name);
    
    /* 简单实现：如果设备名包含在驱动名中则匹配
     * 例如：设备名为"my_device"，驱动名为"my_driver_for_my_device"
     */
    if (strstr(drv->name, dev->kobj.name) != NULL) {
        pr_info("设备 '%s' 与驱动 '%s' 匹配成功\n", 
                dev->kobj.name, drv->name);
        return 1;  /* 匹配成功 */
    }
    
    pr_info("设备 '%s' 与驱动 '%s' 不匹配\n", 
            dev->kobj.name, drv->name);
    return 0;  /* 不匹配 */
}

/* 总线的probe函数，在驱动probe之前调用
 * 可以用于执行总线级别的初始化或检查
 * 返回非零值表示总线probe失败，将阻止驱动probe
 */
static int my_bus_probe(struct device *dev)
{
    pr_info("总线probe函数被调用: 设备=%s\n", dev_name(dev));
    
    /* 在这里可以执行总线级别的初始化
     * 例如，设置设备的电源状态、执行总线专有的配置等
     */
    
    /* 返回0表示总线probe成功，驱动可以继续执行probe */
    return 0;
}

/* 总线的remove函数，在驱动remove之后调用
 * 用于清理总线级别的资源
 * 返回0表示成功，负值表示错误
 */
static int my_bus_remove(struct device *dev)
{
    pr_info("总线remove函数被调用: 设备=%s\n", dev_name(dev));
    
    /* 在这里可以执行总线级别的资源清理
     * 例如，关闭设备的电源、释放总线专有的资源等
     */
    
    return 0; /* 清理成功 */
}

/* 当总线上设备的属性被读取时调用 
 * 函数名必须为<属性名>_show格式，以匹配BUS_ATTR_RO宏的要求
 */
static ssize_t bus_show(struct bus_type *bus, char *buf)
{
    return sprintf(buf, "这是my_bus总线的属性\n");
}

/* 定义总线属性 */
static BUS_ATTR_RO(bus);  /* 使用宏创建只读总线属性 */

/* 定义总线类型 */
static struct bus_type my_bus_type = {
    .name = "my_bus",
    .match = my_bus_match,
    .probe = my_bus_probe,   /* 总线probe函数 */
    .remove = my_bus_remove, /* 总线remove函数 */
};

/* 初始化函数 */
static int __init my_bus_init(void)
{
    int ret;
    
    pr_info("注册my_bus总线...\n");
    
    /* 1. 注册总线类型 */
    ret = bus_register(&my_bus_type);
    if (ret) {
        pr_err("无法注册my_bus总线: %d\n", ret);
        return ret;
    }
    
    /* 2. 创建总线属性文件 */
    ret = bus_create_file(&my_bus_type, &bus_attr_bus);
    if (ret) {
        pr_err("无法创建总线属性: %d\n", ret);
        bus_unregister(&my_bus_type);
        return ret;
    }
    
    pr_info("my_bus总线注册成功\n");
    return 0;
}

/* 退出函数 */
static void __exit my_bus_exit(void)
{
    pr_info("注销my_bus总线...\n");
    
    /* 1. 移除总线属性 */
    bus_remove_file(&my_bus_type, &bus_attr_bus);
    
    /* 2. 注销总线 */
    bus_unregister(&my_bus_type);
    
    pr_info("my_bus总线注销完成\n");
}

module_init(my_bus_init);
module_exit(my_bus_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Simple Bus Registration Demo"); 