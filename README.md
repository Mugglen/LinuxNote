# Linux内核对象模型示例集

这个仓库包含了多个Linux内核模块示例，演示了内核对象模型的不同组件的使用方法。

## 示例列表

### 1. kobject示例 (1_kobject)
演示了如何创建和使用kobject基本操作。

### 2. kset示例 (2_kset)
演示了kset的创建和使用，以及如何将多个kobject组织在kset中。

### 3. kref示例 (3_kref)
演示了kref引用计数机制，用于管理对象的生命周期。

### 4. 属性文件示例 (4_attr)
演示了如何使用`__ATTR`宏创建和管理sysfs属性文件，通过自定义结构体嵌入kobject。

### 5. 自动创建kobject的属性文件示例 (5_attrAuto)
演示了使用`kobject_create_and_add`函数自动创建kobject并管理sysfs属性文件。

## 编译和安装

每个示例目录都包含自己的Makefile和README.md文件，提供了详细的编译和使用说明。

一般步骤：

1. 进入特定示例目录
2. 执行`make`命令编译模块
3. 使用`insmod`加载模块
4. 按照对应README中的指示测试模块功能
5. 使用`rmmod`卸载模块

## 注意事项

这些示例需要在有完整Linux内核源码树的环境中编译。每个Makefile中的内核源码路径(`KDIR`)可能需要根据您的系统进行调整。

## 许可证

GPL 