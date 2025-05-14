# 模块名
obj-m += my_attr_auto.o

# 内核源码路径（根据你的开发板内核实际路径调整）
KDIR := /home/topeet/Linux/kernel

# 当前目录
PWD := $(shell pwd)

# 架构
ARCH := arm64

# 交叉编译器路径（可自定义修改）
CROSS_COMPILE_PATH := /home/topeet/Linux/prebuilts/gcc/linux-x86/aarch64/gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu/bin

CROSS_COMPILE := $(CROSS_COMPILE_PATH)/aarch64-none-linux-gnu-

# 默认目标：编译模块
all:
	make -C $(KDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD) modules

# 清理生成文件
clean:
	rm -rf *.o *.ko *.mod.o *.mod.c *.symvers *.order *.mod Module.symvers modules.order .tmp_versions .*.cmd 