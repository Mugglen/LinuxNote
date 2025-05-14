#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/slab.h>

// 定义kobject类型
static struct kobj_type mytype = {
    .release = NULL, // 简单示例中不实现release
};

// 定义三个kobject变量
static struct kobject *mykobject01;
static struct kobject *mykobject02;
static struct kobject *mykobject03;

static int __init mykobj_init(void)
{
    int ret;
    
    // 方法1: 使用kobject_create_and_add一步创建
    mykobject01 = kobject_create_and_add("mykobject01", NULL);
    if (!mykobject01)
        return -ENOMEM;
    
    // 方法1的变体: 创建子kobject
    mykobject02 = kobject_create_and_add("mykobject02", mykobject01);
    if (!mykobject02) {
        kobject_put(mykobject01);
        return -ENOMEM;
    }
    
    // 方法2: 先分配内存再初始化添加
    mykobject03 = kzalloc(sizeof(*mykobject03), GFP_KERNEL);
    if (!mykobject03) {
        kobject_put(mykobject01);
        kobject_put(mykobject02);
        return -ENOMEM;
    }
    
    ret = kobject_init_and_add(mykobject03, &mytype, NULL, "%s", "mykobject03");
    if (ret) {
        kfree(mykobject03);
        kobject_put(mykobject01);
        kobject_put(mykobject02);
        return ret;
    }
    
    pr_info("Three kobjects created successfully\n");
    return 0;
}

static void __exit mykobj_exit(void)
{
    // 释放所有kobject
    kobject_put(mykobject01);
    kobject_put(mykobject02);
    kobject_put(mykobject03);
    
    pr_info("Kobjects removed\n");
}

module_init(mykobj_init);
module_exit(mykobj_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Simplest kobject example with two creation methods");
