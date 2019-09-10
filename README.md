# 编程工具的集合
## 一、目录结构说明
### 1、debug 目录
* debug目录为显示调试信息的接口集合，实现信息输出的相关接口。
### 2、file 目录
* file目录为文件操作的相关接口集合，实现关于文件的相关接口。
### 3、list 目录
* list目录为链表操作的相关接口集合，实现关于链表的相关接口。
### 4、share_mem 目录
* share_mem目录为共享内存接口集合，对system V共享内存接口的外层封装，方便使用。
### 5、semaphore 目录
* semaphore目录为信号量接口集合，对system V信号量接口的外层封装，方便使用。
### 6、thread_pool 目录
* thread_pool目录为线程池接口集合，对线程池进行了简单的实现，提供了相应接口可以使用。
### 7、scripts 目录
* 存放make menuconfig生成脚本，目前只在X86_64上可以使用。
### 8、serial 目录
* serial目录为串口应用层的设置，打开串口，设置频率、数据位、校验位、停止位等。
### 9、timer 目录
* timer目录为时间操作的接口集合，目前有解决嗜睡（oversleeping）问题的高精度sleep函数clk_nanosleep。

## 二、其他说明
### 1、快速开始
* 先进行make menuconfig 进行集合配置，之后make all生成动态库和静态库。
* 如果不是X86_64平台，导致make menuconfig无效，那么直接配置.config文件即可，例如CONFIG_DEBUG=y，即可将debug目录编译进去。
### 2、项目说明
* 每个集合之间并没耦合，都可单独使用，根据需要选择集合编译进项目即可。
* 接口复杂的集合，会有相应的test.c.test示例代码作为参考。
* 添加make menuconfig，进行集合裁剪，自由配置需要使用的集合。
* 添加make so和make ar，分别生成静态库和动态库。
* 添加make all，生成静态库和动态库。
* 添加make mrproper，删除所有生成文件，包括.config
* 如果有问题，请及时联系我，邮箱qtl_linux@163.com。
### 3、实现目标
* 后续会添加更多的工具集合进来，期望更多的接口实现，便于项目使用。
