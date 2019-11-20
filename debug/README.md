# debug使用说明
### 一、如何使用环境变量，控制日志开关
* 通过make menuconfig 配置，勾选Debug选项后，会弹出Open environment control选项(默认为n)，勾选Open。
* 设置环境变量，export TDEBUG=0或者不设置，则所有接口照常打印。
* TDEBUG=1，Tprint_debug不打印。 Tprint_hex_dump和Tprint_debug情况相同。
* TDEBUG=2，Tprint_debug，Tprint_info_debug不打印。
* TDEBUG=3, Tprint_debug，Tprint_info_debug，Tprint_err_debug不打印。
* 通过make menucofig配置，不勾选Open，编译后，则设置环境变量控制开关无效。
