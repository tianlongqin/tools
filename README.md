# 易于裁剪的编程工具
## 一、目录结构说明
|目录|功能|
|:---|:---|
|images|README.md相关图片。|
|scripts|常用脚本。|
|include|常用头文件，例如list.h双向链表头文件。|
|debug|显示调试信息的接口集合，实现信息输出的相关接口。|
|file|文件操作的相关接口集合，实现关于文件的相关接口。|
|share_mem|共享内存接口集合，对system V共享内存接口的外层封装，方便使用。|
|semaphore|信号量接口集合，对system V信号量接口的外层封装，方便使用。|
|thread_pool|线程池接口集合，对线程池进行了简单的实现，提供了相应接口可以使用。|
|serial|串口应用层的功能接口，包含打开串口，设置频率、数据位、校验位、停止位等。|
|timer|时间操作的接口集合，目前有解决嗜睡（oversleeping）问题的高精度sleep函数clk_nanosleep。注意：使用timer集合的任意函数，在glibc 2.17版本之前都需要连接-lrt动态库。|
|signal|信号操作的接口集合，目前封装了信号阻塞/解除阻塞的函数接口，后续会添加更多的功能接口。|
## 二、其他说明
### 1、快速开始
* 相关依赖ncursesw tinfo bison flex，所以需要安装sudo apt-get install libncurses5-dev libtinfo-dev bison flex。
* 先进行make menuconfig 进行集合配置，之后make all生成动态库和静态库。
### 2、项目说明
* 添加make menuconfig，进行集合裁剪，自由配置需要使用的集合。
* 添加make so和make ar，分别生成静态库和动态库。
* 每个集合之间并没耦合，都可单独使用，根据需要选择集合编译进项目即可。
* 接口复杂的集合，会有相应的test.c.test示例代码作为参考。
* 添加make all，生成静态库和动态库。
* 添加make mrproper，删除所有生成文件，包括.config
* 如果有问题，请及时联系我，邮箱qtl_linux@163.com。
### 3、添加tools到自己的项目中
* 在自己的项目目录下，git clone https://github.com/tianlongqin/tools.git 。clone下tools项目。如图：
 ![CONFIG](https://github.com/tianlongqin/tools/blob/master/images/ls.png)
* 进入tools目录，进行裁剪编译。make menuconfig，我只使用debug和serial目录。如图：
 ![CONFIG](https://github.com/tianlongqin/tools/blob/master/images/menuconfig1.png)
* make all，生成静态库和动态库，目录文件存放在build目录下。如图：
 ![CONFIG](https://github.com/tianlongqin/tools/blob/master/images/makeall.png)
* makefile中添加include路径和libs路径。如图:
 ![CONFIG](https://github.com/tianlongqin/tools/blob/master/images/makefile.png)
* 在src文件中如何使用tools的头文件，include <dir/file.h>格式，如图：
 ![CONFIG](https://github.com/tianlongqin/tools/blob/master/images/incldue.png)
* 准备工作已经完成，可以使用tools接口，然后进行编译即可。如图：
 ![CONFIG](https://github.com/tianlongqin/tools/blob/master/images/make.png)
 * 使用strings build/libtool.so | grep CONFIG 可以查看配置。如图：
 ![CONFIG](https://github.com/tianlongqin/tools/blob/master/images/CONFIG1.png)
 * tools_test路径：https://github.com/tianlongqin/tools_test.git
### 4、实现目标
* 后续会添加更多的工具集合进来，期望更多的接口实现，便于项目使用。
