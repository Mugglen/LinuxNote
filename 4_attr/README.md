# SYSFS 属性文件示例

这个示例演示了如何在Linux内核模块中使用`__ATTR`宏创建和管理sysfs属性文件。

## 功能说明

本模块实现了以下功能：

1. 创建一个模拟设备，包含两个属性：`value`（整数）和`name`（字符串）
2. 使用`__ATTR`宏定义这些属性的读写处理函数
3. 通过sysfs文件系统向用户空间暴露这些属性
4. 允许用户通过读写sysfs文件来查看和修改设备属性

## 代码亮点

- 演示了`__ATTR`宏的使用，这是简化属性定义的常用方法
- 展示了`container_of`宏从kobject获取父结构体的用法
- 实现了完整的属性读写处理函数
- 使用属性组（attribute group）管理多个属性

## 编译和安装

在此目录下执行：

```
make
```

加载模块：

```
insmod my_attr.ko
```

## 使用方法

模块加载后，将在`/sys/my_attr_demo/`创建两个属性文件：

- `value` - 保存整数值
- `name` - 保存字符串

### 读取属性值

```
cat /sys/my_attr_demo/value
cat /sys/my_attr_demo/name
```

### 修改属性值

```
echo "200" > /sys/my_attr_demo/value
echo "new_device_name" > /sys/my_attr_demo/name
```

## 卸载模块

```
rmmod my_attr
``` 