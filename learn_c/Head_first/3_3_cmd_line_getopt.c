#include<stdio.h>
#include<unistd.h>//使用命令行 解析函数 geopt()函数 

// ./ord -d now -t pineapple apple as
int main(int argc, char *argv[]){//可使用命令行参数 

    char *delivery = "";
	int tick = 0;
	int count = 0;
	char ch;
	
	while((ch=getopt(argc, argv, "d:t"))!=EOF){//命令行中需要 选项 -d  且:代表 -d选项需要参数 还有 -t选项 
		switch(ch){
			case 'd'://-d 选项 
				delivery = optarg;
				break;
			case 't'://-t选项 
				tick = 1;
				break;
			default://其他选项 错误 
				fprintf(stderr,"未知选项\n");
				return 1;
			
		} 
	}
	argc -= optind;//计数 optind中保存了 移位 
	argv += optind;//指针移位 
	
	if(tick) puts("Tick crust.");
	if(delivery[0] ) printf("To be delivered %s.\n",delivery);
	puts("其他命令行参数");
	for(count=0;count<argc;count++)//getopt()程序处理之后 第一个参数就变成了 argv[0] 
		puts(argv[count]); 
	
	return 0;
} 
