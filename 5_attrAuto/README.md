# 自动创建Kobject的SYSFS属性文件示例

这个示例演示了如何使用`kobject_create_and_add`函数自动创建kobject并管理sysfs属性文件。

## 与前一个示例的区别

与4_attr示例相比，本示例的主要区别在于:

1. 使用`kobject_create_and_add`直接创建一个独立的kobject，而不是嵌入到自定义结构体中
2. 使用全局变量存储属性值，而不是结构体成员
3. 简化了kobject的生命周期管理
4. 无需定义自己的release回调函数

## 功能说明

本模块实现了以下功能：

1. 通过`kobject_create_and_add`自动创建kobject
2. 创建两个属性：`value`（整数）和`name`（字符串）
3. 使用`__ATTR`宏定义这些属性的读写处理函数
4. 通过sysfs文件系统向用户空间暴露这些属性

## 代码亮点

- 演示了`kobject_create_and_add`函数的用法
- 对比了与手动创建kobject方式的区别
- 更简洁的属性读写处理逻辑
- 更简单的初始化和清理代码

## 编译和安装

在此目录下执行：

```
make
```

加载模块：

```
insmod my_attr_auto.ko
```

## 使用方法

模块加载后，将在`/sys/my_attr_auto/`创建两个属性文件：

- `value` - 保存整数值
- `name` - 保存字符串

### 读取属性值

```
cat /sys/my_attr_auto/value
cat /sys/my_attr_auto/name
```

### 修改属性值

```
echo "200" > /sys/my_attr_auto/value
echo "new_device_name" > /sys/my_attr_auto/name
```

## 卸载模块

```
rmmod my_attr_auto
``` 