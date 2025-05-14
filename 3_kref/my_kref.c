#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/kref.h>
#include <linux/slab.h>

/* 
 * kref是Linux内核中的引用计数机制，用于跟踪对象的使用情况
 * 当引用计数降至0时，对象可以被安全释放
 */

// 定义一个包含kref的自定义结构体
struct my_device {
    struct kref kref;      // 嵌入kref结构体
    char name[32];         // 设备名称
    // 其他设备相关数据...
};

// 当引用计数降为0时的释放函数
static void my_device_release(struct kref *kref)
{
    // 使用container_of宏获取包含kref的my_device对象指针
    struct my_device *dev = container_of(kref, struct my_device, kref);
    
    pr_info("Releasing device: %s\n", dev->name);
    // 执行清理操作
    kfree(dev);
}

// 获取设备引用的函数
static struct my_device *my_device_get(struct my_device *dev)
{
    if (dev) {
        // 增加引用计数
        kref_get(&dev->kref);
        pr_info("Increased reference count for device: %s\n", dev->name);
    }
    return dev;
}

// 释放设备引用的函数
static void my_device_put(struct my_device *dev)
{
    if (dev) {
        pr_info("Decreasing reference count for device: %s\n", dev->name);
        // 减少引用计数，如果降为0，自动调用my_device_release释放
        kref_put(&dev->kref, my_device_release);
    }
}

// 创建新设备的函数
static struct my_device *my_device_create(const char *name)
{
    struct my_device *dev;
    
    dev = kzalloc(sizeof(*dev), GFP_KERNEL);
    if (!dev)
        return NULL;
    
    // 初始化引用计数为1
    kref_init(&dev->kref);
    
    // 设置设备名称
    strncpy(dev->name, name, sizeof(dev->name) - 1);
    
    pr_info("Created new device: %s with initial reference count 1\n", dev->name);
    return dev;
}

// 全局变量存储设备指针
static struct my_device *mydev;

static int __init mykref_init(void)
{
    pr_info("kref示例模块初始化\n");
    
    // 创建设备（初始引用计数为1）
    mydev = my_device_create("sample_device");
    if (!mydev) {
        pr_err("创建设备失败\n");
        return -ENOMEM;
    }
    
    // 模拟增加引用计数的场景
    my_device_get(mydev);  // 引用计数增至2
    my_device_get(mydev);  // 引用计数增至3
    
    pr_info("kref示例初始化完成\n");
    return 0;
}

static void __exit mykref_exit(void)
{
    pr_info("kref示例模块退出\n");
    
    if (mydev) {
        // 模拟减少引用的场景
        my_device_put(mydev);  // 引用计数降至2
        my_device_put(mydev);  // 引用计数降至1
        my_device_put(mydev);  // 引用计数降至0，触发释放
    }
    
    pr_info("kref示例退出完成\n");
}

module_init(mykref_init);
module_exit(mykref_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("kref引用计数示例模块");

