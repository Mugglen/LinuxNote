#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/slab.h>

// 定义kobject指针
static struct kobject *mykobject01;
static struct kobject *mykobject02;

// 定义kset指针
static struct kset *mykset;

// 定义kobject类型
static struct kobj_type mytype = {
    .release = NULL, // 简单示例中不实现release
};

static int __init mykobj_init(void)
{
    int ret;
    
    pr_info("Kset test module init\n");
    
    // 1. 创建kset
    mykset = kset_create_and_add("mykset", NULL, NULL);
    if (!mykset) {
        pr_err("Failed to create kset\n");
        return -ENOMEM;
    }
    
    // 2. 创建第一个kobject并加入kset
    mykobject01 = kzalloc(sizeof(*mykobject01), GFP_KERNEL);
    if (!mykobject01) {
        pr_err("Failed to allocate mykobject01\n");
        goto err_kset;
    }
    
    mykobject01->kset = mykset; // 设置所属kset
    ret = kobject_init_and_add(mykobject01, &mytype, NULL, "%s", "mykobject01");
    if (ret) {
        pr_err("Failed to init and add mykobject01\n");
        goto err_kobj01;
    }
    
    // 3. 创建第二个kobject并加入同一个kset
    mykobject02 = kzalloc(sizeof(*mykobject02), GFP_KERNEL);
    if (!mykobject02) {
        pr_err("Failed to allocate mykobject02\n");
        goto err_kobj01_added;
    }
    
    mykobject02->kset = mykset; // 设置所属kset
    ret = kobject_init_and_add(mykobject02, &mytype, NULL, "%s", "mykobject02");
    if (ret) {
        pr_err("Failed to init and add mykobject02\n");
        goto err_kobj02;
    }
    
    pr_info("Kset and kobjects created successfully\n");
    return 0;

// 错误处理
err_kobj02:
    kfree(mykobject02);
err_kobj01_added:
    kobject_put(mykobject01);
err_kobj01:
    kfree(mykobject01);
err_kset:
    kset_unregister(mykset);
    return -ENOMEM;
}

static void __exit mykobj_exit(void)
{
    pr_info("Kset test module exit\n");
    
    // 释放资源
    kobject_put(mykobject01);
    kobject_put(mykobject02);
    kset_unregister(mykset);
    
    pr_info("Kset and kobjects removed\n");
}

module_init(mykobj_init);
module_exit(mykobj_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Kset test module");
