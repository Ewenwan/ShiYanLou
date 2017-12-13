// 字符串加密解密 函数 encrypt.c
#include "encrypt.h"

void encrypt(char *message){
	while(*message){//取地址内的值
		*message = *message ^ 31;
		message++;//指针++
	}
}
// encrypt.h
void encrypt(char *message);
// 编译 gcc -c encrypt.c -o encrypt.o

// 核查函数 检查字符串是否被篡改 checksum.c
#include "checksum.h"

int checksum(char *message){
	int c=0;
	while(*message){
	c +=  c ^ (int)(*message);
	message++;
	}
}
// checksum.h
int checksum(char *message);
// 编译 gcc -c checksum.c -o checksum.o

//主函数文件 test.c
#include <stdio.h>// <> 尖括号表示标准头文件 在 /usr/local/include
                  // /usr/include 下搜索  linux
#include "checksum.h"//调用自己的库文件(放在当前目录下) 
#include "encrypt.h"//需要使用 " " 
//或放在其他地方 #include "my_header_file/encrypt.h"
// 也可以在编译时 -I 指定目录  gcc -I/my_header_file test.c -o test
// 也可以放在标准头文件下 /usr/local/include 使用 尖括号<>  
  
int main(){
	char s[]="Speak friend and enter"; //字符数组
	encrypt(s);
	printf("加密后的字符串'%s'\r\n",s);
	printf("核对数值为%i",checksum(s));
	encrypt(s);
	printf("解密后的字符串'%s'\r\n",s);
	printf("核对数值为%i",checksum(s));
	return 0;
}

// 创建存档文件  ar 命令 libXXX.a  
ar -rcs libhfsecurity.a encrypt.o checksum.o
// 使用存档文件 编译程序 hfsecurity找 libhfsecurity.a的存档文件
gcc test.c -lhfsecurity -o test
gcc test.c -L/my_lib -lhfsecurity -o test 
// -L指定 去哪个目录 /my_lib  找存档文件
// nm 命令查看存档文件 libXXX.a  中包含的 .o目标文件以及 函数


// 使用makefile文件  bank_vault.c  使用了  checksum.h  "encrypt.h"

encrypt.o: encrypt.c
	gcc -c encrypt.c -o encrypt.o
	
checksum.o: checksum.c	
	gcc -c checksum.c -o checksum.o
	
libhfsecurity.a: encrypt.o checksum.o
	ar -rcs libhfsecurity.a encrypt.o checksum.o
	
bank_vault bank_vault.c libhfsecurity.a
	gcc bank_vault.c -I . -L . -lhfsecurity -o bank_vault

// 	-I 指定 源文件目录  -L 指定存档文件目录
	


	
// 创建动态链接库  
// hfcal.h 在 /include 下  -fPIC 创建位置无关代码 
// -c 不要链接代码
编译为 目标文件    gcc -I/include -fPIC -c hfcal.c -o 	hfcal.o
转化为共享目标文件 gcc -shared hfcal.o -o  c:\libs\hfcal.dll   window下
gcc -shared hfcal.o -o   /libs/hfcal.so     linux下
gcc -shared hfcal.o -o   /libs/hfcal.dylib  MAC下 
老的mac下 可能无 -shared  需要 -dynamiclib

//使用动态库
gcc -I/include -c elliptical.c -o elliptical.o
gcc elliptical.o -L/libs -lhfcal -o elliptical
//  公开 目录
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/libs
				