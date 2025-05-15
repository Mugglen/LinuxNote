# 模块名
obj-m += deviceRegist.o driverRegist.o my_bus.o
# obj-m += my_device.o my_driver.o  # 暂时不编译设备和驱动模块

# 内核源码路径（根据你的开发板内核实际路径调整）
KDIR := /home/topeet/Linux/kernel

# 当前目录
PWD := $(shell pwd)

# 架构
ARCH := arm64

# 交叉编译器路径（可自定义修改）
CROSS_COMPILE_PATH := /home/topeet/Linux/prebuilts/gcc/linux-x86/aarch64/gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu/bin

CROSS_COMPILE := $(CROSS_COMPILE_PATH)/aarch64-none-linux-gnu-

# 安装目录（可以根据需要修改）
INSTALL_MOD_PATH := /home/LF/9_registerDriver/modules

# 默认目标：编译所有模块
all:
	make -C $(KDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD) modules

# 安装模块到指定目录
install: all
	@echo "正在安装模块到 $(INSTALL_MOD_PATH)..."
	@mkdir -p $(INSTALL_MOD_PATH)
	@cp -f *.ko $(INSTALL_MOD_PATH)/
	@echo "模块安装完成"

# 清理生成文件
clean:
	rm -rf *.o *.ko *.mod.o *.mod.c *.symvers *.order *.mod Module.symvers modules.order .tmp_versions .*.cmd 
	rm -rf $(INSTALL_MOD_PATH)/*.ko

# 加载模块到内核（需要root权限）
load: all
	@echo "正在加载模块到内核..."
	@sudo insmod my_bus.ko
	@sudo insmod deviceRegist.ko
	@sudo insmod driverRegist.ko
	@echo "模块加载完成"

# 卸载模块
unload:
	@echo "正在卸载模块..."
	@sudo rmmod driverRegist
	@sudo rmmod deviceRegist
	@sudo rmmod my_bus
	@echo "模块卸载完成"

# 重新加载模块（先卸载再加载）
reload: unload load

# 更新help信息
help:
	@echo "可用的目标："
	@echo "  all     - 编译所有模块（默认目标）"
	@echo "  install - 编译并安装模块到 $(INSTALL_MOD_PATH)"
	@echo "  load    - 编译并加载模块到内核（需要root权限）"
	@echo "  unload  - 从内核卸载模块（需要root权限）"
	@echo "  reload  - 重新加载模块（先卸载再加载）"
	@echo "  clean   - 清理所有生成的文件"
	@echo "  help    - 显示此帮助信息"