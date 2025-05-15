# 设备注册示例

这个示例演示了如何在Linux内核中将设备注册到自定义总线上。

## 功能概述

- 在`my_bus`总线上注册一个名为`my_device`的设备
- 实现设备的初始化和释放

## 文件说明

- `deviceRegist.c`: 设备注册模块源代码
- `Makefile`: 用于编译内核模块的make脚本

## 使用方法

### 编译

```bash
make
```

### 加载模块

**注意**: 必须先加载my_bus模块才能加载此设备模块

```bash
# 手动加载
insmod ../7_bus2/my_bus.ko
insmod deviceRegist.ko
```

### 查看设备

```bash
# 查看设备列表
ls /sys/bus/my_bus/devices/
```

### 卸载模块

```bash
rmmod deviceRegist
rmmod my_bus
```

## 源码分析

### 设备注册流程

1. 为设备分配内存
2. 设置设备名称和释放函数
3. 分配和初始化设备私有数据
4. 将设备与总线关联
5. 注册设备 