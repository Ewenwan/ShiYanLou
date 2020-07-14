# 日常调试

> coredump查询死机问题:

使系统将coredump输出到文件，而不是上报给系统的处理程序 echo core > /proc/sys/kernel/core_pattern

然后adb coredump文件 分析死机的代码位置
