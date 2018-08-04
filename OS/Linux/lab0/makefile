# 变量定义 ( = or := )
# 其中 = 和 := 的区别在于, 
# := 只能使用前面定义好的变量, = 可以使用后面定义的变量
# +=变量追加值 SRCS += programD.c

# 工程名
# 定义变量PROJ为 challenge ,在后面 handin 中使用了这个变量，将其插入生成的压缩包名字中
# 可能是用来让同学改为学号等信息对提交的作业进行区分
PROJ	:= challenge
# 生成空格
EMPTY	:=
SPACE	:= $(EMPTY) $(EMPTY)
# 斜杠 /   反斜杠\ back slash
SLASH	:= /

# 后面没有使用上面的3个变量

# @放在行首，表示不打印此行信息， at符号
V       := @
# 显示信息
#V       :=
# 为了显示所有执行的命令，我一开始是把Line6的“V:=@”改为“V:=”，然后make。
# 不过后来在网上看到，可以通过执行make "V=" 来达到目的。
# make qemu "V="
#变量V=@，后面大量使用了V
#@的作用是不输出后面的命令，只输出结果
#在这里修改V即可调整输出的内容
#也可以 make "V=" 来完整输出

# 不输出警告信息
W:=
# 输出警告信息
# W:= -Wall
#为了不输出warning，我自己加的


# 编译器=========================

#need llvm/clang-3.5+
#USELLVM := 1
#若要使用LLVM则去掉前面一行的#即可
#LLVM 是LLVM基金会开发的编译器架构，Clang是其开发的C++，C，ObjectiveC,Ojc++编译器。

# 这里是在选择交叉编译器。
# try to infer the correct GCCPREFX
# 检查环境变量 GCCPREFIX 是否被设置（通常是没有的）
ifndef GCCPREFIX
# 如果没有被设置(if not define)，那么判断运行环境，自行定义变量 GCCPREFIX
# grep 在文本信息中查找 
# 0 是一个文件描述符，表示标准输入(stdin) == keyboard 键盘输入,并返回在前端 =========
# 1 是一个文件描述符，表示标准输出(stdout)== monitor 正确返回值 输出到前端  ====
# 2 是一个文件描述符，表示标准错误(stderr)== monitor 错误返回值 输出到前端  ====
# >和>> 都是重定向输出=== > 会覆盖已有的文件内容，而 >> 会附加到已有内容之后====
# 1>   指 标准信息输出路径（也就是默认的输出方式）   "1>" 通常可以省略成 ">". 
# 2>   指 错误信息输出路径                      
# 2>&1 指将 标准错误 指定 为标准输出（错误合并到输出） &1 表示 标准输出通道
# 1>&2 正确返回值传递给 2输出通道 &2表示 输出错误通道 (正确合并到错误)
# 如果此处错写成 1>2, 就表示把1输出重定向到文件2中. 
# <和<<都是重定向输入===========
# <0指标准输入路径
# 4<&0 指的是将文件描述符4指定为标准输入（实际可选4到9之间任意一个数字）

GCCPREFIX := $(shell if i386-elf-objdump -i 2>&1 | grep '^elf32-i386$$' >/dev/null 2>&1; \
	then echo 'i386-elf-'; \
	elif objdump -i 2>&1 | grep 'elf32-i386' >/dev/null 2>&1; \
	then echo ''; \
	else echo "***" 1>&2; \
	echo "*** Error: Couldn't find an i386-elf version of GCC/binutils." 1>&2; \
	echo "*** Is the directory with i386-elf-gcc in your PATH?" 1>&2; \
	echo "*** If your i386-elf toolchain is installed with a command" 1>&2; \
	echo "*** prefix other than 'i386-elf-', set your GCCPREFIX" 1>&2; \
	echo "*** environment variable to that prefix and run 'make' again." 1>&2; \
	echo "*** To turn off this error, run 'gmake GCCPREFIX= ...'." 1>&2; \
	echo "***" 1>&2; exit 1; fi)
endif

# 硬件模拟环境====================
# QEMU是一款优秀的模拟处理器，使用方便，比virtualbox更适合进行实验。
# try to infer the correct QEMU
# 检查环境变量 QEMU 是否被设置（通常是没有的）
ifndef QEMU
# 如果没有被设置(if not define)，那么自行定义变量 QEMU
# /dev/null 哑型设备  无用信息收集器 (不会打印信息，哑巴)
# which 查找 并 显示 给定 命令 的绝对路径
QEMU := $(shell if which qemu-system-i386 > /dev/null; \
	then echo 'qemu-system-i386'; exit; \
	elif which i386-elf-qemu > /dev/null; \
	then echo 'i386-elf-qemu'; exit; \
	elif which qemu > /dev/null; \
	then echo 'qemu'; exit; \
	else \
	echo "***" 1>&2; \
	echo "*** Error: Couldn't find a working QEMU executable." 1>&2; \
	echo "*** Is the directory containing the qemu binary in your PATH" 1>&2; \
	echo "***" 1>&2; exit 1; fi)
endif

# 使用伪目标.SUFFIXES 定义自己的后缀列表
# 当前makefile内 支持 文件后缀 的类型列表
# eliminate default suffix rules
.SUFFIXES: .c .S .h

# 如果编译出错，或者编译中断，删除已经生成的目标文件
# delete target files if there is an error (or make is interrupted)
.DELETE_ON_ERROR:

# 设置编译器选项
# define compiler and flags 编译器和编译选项
ifndef  USELLVM
# 未定义 USELLVM 则使用 gcc编译器
# /dev/null 哑型设备  无用信息收集器 (不会打印信息，哑巴)

# hostcc是给主机用的编译器，按照主机格式。

HOSTCC		:= gcc

 
# -g 是为了gdb能够对程序进行调试  GNU  debug
# -Wall 生成警告信息
# -O2 优化处理（0,1,2,3表示不同的优化程度，O0为不优化）
HOSTCFLAGS	:= -g -Wall -O2

# cc 是 i386、elf32 格式的编译器 (交叉编译器)。

CC		:= $(GCCPREFIX)gcc
 
# -fno-builtin 不接受非“__”开头的 内建函数 buildin function
# -Wall 生成警告信息
# -ggdb 让 gcc 为 gdb 生成 比较丰富 的 调试信息
# -m32  编译32位程序 
# -gstabs 此选项以 stabs 格式声称调试信息, 但是不包括gdb调试信息
# -nostdinc 不在 标准系统 目录中搜索头文件, 只在-I指定的目录中搜索   no standard include
# DEFS是未定义量。可用来对CFLAGS进行扩展。
CFLAGS	:= -fno-builtin -Wall -ggdb -m32 -gstabs -nostdinc  $(DEFS)


 
# 这句话的意思是，如果-fno-stack-protector(无堆栈保护??)选项存在，就添加它。过程蛮复杂的。
# -fstack-protector-all 启用 堆栈保护, 为所有函数 插入保护代码
# -E 仅作预处理，不进行编译、汇编和链接
# -x c 指明使用的语言为 c语言
# 前一个 /dev/null 用来指定 目标文件
# >/dev/null 2>&1 将标准输出与错误输出重定向到 /dev/null(哑型设备  无用信息收集器 (不会打印信息，哑巴))
# /dev/null是一个 垃圾桶 一样的东西
# ‘&&’之前的半句表示，试着对一个垃圾 跑一下这个命令，所有的 输出 都作为垃圾，为了快一点，开了-E。
# 如果不能运行，那么&&前面的条件不成立，后面的就被忽视。======================s
# 如果可以运行，那么&&后面的句子得到执行，于是 CFLAGS += -fno-stack-protector
CFLAGS	+= $(shell $(CC) -fno-stack-protector -E -x c /dev/null >/dev/null 2>&1 && echo -fno-stack-protector)

else
# 如果定义了 USELLVM 则使用 clang编译器
# LLVM 是LLVM基金会开发的编译器架构，Clang是其开发的 C++，C，ObjectiveC, Ojc++编译器。

HOSTCC		:= clang
HOSTCFLAGS	:= -g -Wall -O2
CC		:= clang
CFLAGS	:= -fno-builtin -Wall -g -m32 -mno-sse -nostdinc $(DEFS)
CFLAGS	+= $(shell $(CC) -fno-stack-protector -E -x c /dev/null >/dev/null 2>&1 && echo -fno-stack-protector)
endif

# 源文件类型为 c 和 S=======
# .c文件 .S汇编文件
CTYPE	:= c S

# 一些链接选项========
LD      := $(GCCPREFIX)ld
# #ld -V命令会输出连接器的版本与支持的模拟器。在其中搜索grep elf_i386， 错误信息到 哑型设备
# 若支持，则LDFLAGS := -m elf_i386
LDFLAGS	:= -m $(shell $(LD) -V | grep elf_i386 2>/dev/null)

# -nostdinc 不在 标准系统 目录中搜索头文件, no standard include
# 只把指定的文件传递给连接器
LDFLAGS	+= -nostdlib

# objcopy把一种目标文件中的内容复制到另一种类型的目标文件中. 
OBJCOPY := $(GCCPREFIX)objcopy
# objdump命令是Linux下的反汇编目标文件或者可执行文件的命令
OBJDUMP := $(GCCPREFIX)objdump
# shell 指令
COPY	:= cp#复制
MKDIR   := mkdir -p#创建文件夹  make directory
MV	:= mv      #移动文件    move 
RM	:= rm -f   #删除文件    remove 
AWK	:= awk     #逐行逐列处理      字符串解析
SED	:= sed     #sed常常一整行处理 
SH	:= sh      #bash解析命令      
TR	:= tr      #对来自标准输入的字符进行替换、压缩和删除
TOUCH	:= touch -c#-c 如果文件不存在,则不要进行创建

OBJDIR	:= obj# 目标地址目标
BINDIR	:= bin# 二进制文件地址

# 可执行文件
ALLOBJS	:=
# 依赖
ALLDEPS	:=
# 目标
TARGETS	:=

#包含另外一个Makefile文件
#function.mk中定义了大量的函数。
#.mk中每个函数都有注释。
include tools/function.mk

#call函数：call func,变量1，变量2,...
#listf函数在function.mk中定义，列出某地址（变量1）下某些类型（变量2）文件
#listf_cc函数即列出某地址（变量1）下.c与.S文件
listf_cc = $(call listf,$(1),$(CTYPE))

# for cc
# add_files:(#files, cc[, flags, packet, dir])
# add_files_cc:(#files, packet, flags, dir) flags已添加，这个变量仅用以扩展
# 添加文件到目标
add_files_cc = $(call add_files,$(1),$(CC),$(CFLAGS) $(3),$(2),$(4))
create_target_cc = $(call create_target,$(1),$(2),$(3),$(CC),$(CFLAGS))

# for hostcc
add_files_host = $(call add_files,$(1),$(HOSTCC),$(HOSTCFLAGS),$(2),$(3))
create_target_host = $(call create_target,$(1),$(2),$(3),$(HOSTCC),$(HOSTCFLAGS))


#patsubst替换通配符
#cgtype（filenames,type1，type2）
#把文件名中 后缀是 type 1的改为 type2， 如*.c改为*.o
cgtype = $(patsubst %.$(2),%.$(3),$(1))
 
# 列出所有.o文件
objfile = $(call toobj,$(1))
# .o改为.asm
asmfile = $(call cgtype,$(call toobj,$(1)),o,asm)
# .o改为.out
outfile = $(call cgtype,$(call toobj,$(1)),o,out)
# .o改为.sym
symfile = $(call cgtype,$(call toobj,$(1)),o,sym)

# for match pattern
match = $(shell echo $(2) | $(AWK) '{for(i=1;i<=NF;i++){if(match("$(1)","^"$$(i)"$$")){exit 1;}}}'; echo $$?)

# >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# include kernel/user
#库文件 
# +=变量追加值
INCLUDE	+= libs/

CFLAGS	+= $(addprefix -I,$(INCLUDE))

LIBDIR	+= libs

$(call add_files_cc,$(call listf_cc,$(LIBDIR)),libs,)

# -------------------------------------------------------------------
# kernel  内核源文件
# 生成 bin/kernel
# 头文件
KINCLUDE	+= kern/debug/ \
		   kern/driver/ \
		   kern/trap/ \
		   kern/mm/
# 源文件
KSRCDIR		+= kern/init \
		   kern/libs \
		   kern/debug \
		   kern/driver \
		   kern/trap \
		   kern/mm

KCFLAGS		+= $(addprefix -I,$(KINCLUDE))

$(call add_files_cc,$(call listf_cc,$(KSRCDIR)),kernel,$(KCFLAGS))

#应为所有的编译后的目标文件路径都保存在__temp_packet中，则该函数直接引用，用来最后的链接工作
KOBJS	= $(call read_packet,kernel libs)

# create kernel target  系统内核目标文件 obj/kernel
kernel = $(call totarget,kernel)

# 最终的目标文件的规则
$(kernel): tools/kernel.ld
# 命令前缀 
# 前缀 @   :: 只输出命令执行的结果, 出错的话停止执行
# 前缀 -   :: 命令执行有错的话, 忽略错误, 继续执行
$(kernel): $(KOBJS)
	@echo + ld $@
# 链接 obj/libs/* 和 obj/kernel/init/* ... 所有的目标文件生成 elf-i386 的内核文件,并且使用kernel.ld链接器脚本
	$(V)$(LD) $(LDFLAGS) -T tools/kernel.ld -o $@ $(KOBJS)
# 最终的内核文件应该去除符号表等信息，并输出符号表信息，汇编文件信息，和输出信息
	@$(OBJDUMP) -S $@ > $(call asmfile,kernel)
	@$(OBJDUMP) -t $@ | $(SED) '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > $(call symfile,kernel)

$(call create_target,kernel)

# -------------------------------------------------------------------

# create bootblock 引导区
# 生成 bin/bootblock
# 启动扇区的编译，过程与内核差不多唯一的区别是需要对编译后的启动扇区进行签名，即有效启动扇区，最后字节为0x55aa。
bootfiles = $(call listf_cc,boot)# boot　下的文件 
$(foreach f,$(bootfiles),$(call cc_compile,$(f),$(CC),$(CFLAGS) -Os -nostdinc))

bootblock = $(call totarget,bootblock)

$(bootblock): $(call toobj,$(bootfiles)) | $(call totarget,sign)
	@echo + ld $@
	$(V)$(LD) $(LDFLAGS) -N -e start -Ttext 0x7C00 $^ -o $(call toobj,bootblock)
	@$(OBJDUMP) -S $(call objfile,bootblock) > $(call asmfile,bootblock)
	@$(OBJCOPY) -S -O binary $(call objfile,bootblock) $(call outfile,bootblock)
	@$(call totarget,sign) $(call outfile,bootblock) $(bootblock)
# boot/bootasm.S  ----> bootasm.o
# boot/bootmain.c ----> bootmain.o
# bootasm.o + bootmain.o ------> bootblock
$(call create_target,bootblock)

# -------------------------------------------------------------------
# 生成 tools/sign.c ----> bin/sign
# create 'sign' tools　创建符合规定的硬盘引导扇区　 bootblock.out + 0x55AA -> bootblock (512字节)
# 在内核工具目录中,sign.c， 用来给扇区签名的小工具, 为什么这而使用host呢，
# 是因为该工具是在特定操作系统下的工具，所以编译过程跟内核编译过程完全不同，
# 最显著的就是 nostdlibc 内核是必须的编译选项，
# 而应用软件一般都是依赖C库，并且内核代码为了精简，
# 也没有栈溢出保护 --no-stack-protector
$(call add_files_host,tools/sign.c,sign,sign)
$(call create_target_host,sign,sign)
# 生成 一个被系统认为是符合规范的硬盘主引导扇区
# 主引导区大小为512字节且最后两个字节为 0x55 和 0xAA，只要达到这两个条件即符合规范。
# buf[510] = 0x55
# buf[511] = 0xAA
 

# -------------------------------------------------------------------
# 生成 bin/ucore.img
#最后把编译出的二进制文件和bootloader都写进一个大文件中，用来模拟硬盘。使用linux下dd块命令
# create ucore.img　　创建虚拟硬盘文件
# 引导扇区bootblock + 系统内核kernel -> ucore.img
UCOREIMG	:= $(call totarget,ucore.img)

$(UCOREIMG): $(kernel) $(bootblock)
	$(V)dd if=/dev/zero of=$@ count=10000
	$(V)dd if=$(bootblock) of=$@ conv=notrunc
	$(V)dd if=$(kernel) of=$@ seek=1 conv=notrunc

$(call create_target,ucore.img)

# >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# 收尾工作/定义变量
$(call finish_all)

IGNORE_ALLDEPS	= clean \
		  dist-clean \
		  grade \
		  touch \
		  print-.+ \
		  handin

ifeq ($(call match,$(MAKECMDGOALS),$(IGNORE_ALLDEPS)),0)
-include $(ALLDEPS)
endif

# files for grade script
# 定义各种make目标
TARGETS: $(TARGETS)

.DEFAULT_GOAL := TARGETS

.PHONY: qemu qemu-nox debug debug-nox
#下面的很多命令是qemu的参数，其中一些网上不太好查，有兴趣的话可以去 http://wiki.qemu.org/Manual 查阅

#终端模式打开qemu   -monitor
qemu-mon: $(UCOREIMG)
	$(V)$(QEMU)  -no-reboot -monitor stdio -hda $< -serial null

#新窗口下打开qemu   -parallel
qemu: $(UCOREIMG)
	$(V)$(QEMU) -no-reboot -parallel stdio -hda $< -serial null

 
#运行并生成log文件
log: $(UCOREIMG)
	$(V)$(QEMU) -no-reboot -d int,cpu_reset  -D q.log -parallel stdio -hda $< -serial null

 
#禁止图形界面，转到终端
qemu-nox: $(UCOREIMG)
	$(V)$(QEMU)   -no-reboot -serial mon:stdio -hda $< -nographic
TERMINAL        :=gnome-terminal


#调试
# 利用make debug来观察BIOS的单步执行
# 首先是对qemu进行的操作
# sleep 2　等待一段时间
# 针对 gdbinit 文件进行的调试
debug: $(UCOREIMG)
	$(V)$(QEMU) -S -s -parallel stdio -hda $< -serial null &
	$(V)sleep 2
	$(V)$(TERMINAL) -e "gdb -q -tui -x tools/gdbinit"

#在终端打开qemu进行调试，现在终端会陷入死循环QAQ
debug-nox: $(UCOREIMG)
	$(V)$(QEMU) -S -s -serial mon:stdio -hda $< -nographic &
	$(V)sleep 2
	$(V)$(TERMINAL) -e "gdb -q -x tools/gdbinit"

.PHONY: grade touch

GRADE_GDB_IN	:= .gdb.in
GRADE_QEMU_OUT	:= .qemu.out
HANDIN			:= proj$(PROJ)-handin.tar.gz

TOUCH_FILES		:= kern/trap/trap.c

MAKEOPTS		:= --quiet --no-print-directory

grade:
	$(V)$(MAKE) $(MAKEOPTS) clean
	$(V)$(SH) tools/grade.sh

touch:
	$(V)$(foreach f,$(TOUCH_FILES),$(TOUCH) $(f))

print-%:
	@echo $($(shell echo $(patsubst print-%,%,$@) | $(TR) [a-z] [A-Z]))

.PHONY: clean dist-clean handin packall tags
# 清理
clean:
	$(V)$(RM) $(GRADE_GDB_IN) $(GRADE_QEMU_OUT) cscope* tags
	-$(RM) -r $(OBJDIR) $(BINDIR)
#把压缩包也删除
dist-clean: clean
	-$(RM) $(HANDIN)

#打包并输出一句话
handin: packall
	@echo Please visit http://learn.tsinghua.edu.cn and upload $(HANDIN). Thanks!

#打包
packall: clean
	@$(RM) -f $(HANDIN)
	@tar -czf $(HANDIN) `find . -type f -o -type d | grep -v '^\.*$$' | grep -vF '$(HANDIN)'`

#可能是输出所有tags，要cscope工具
tags:
	@echo TAGS ALL
	$(V)rm -f cscope.files cscope.in.out cscope.out cscope.po.out tags
	$(V)find . -type f -name "*.[chS]" >cscope.files
	$(V)cscope -bq 
	$(V)ctags -L cscope.files
