#include<stdio.h>
#include<stdlib.h>
#include<string.h>
 
// gcc file_flow.c -o categorize && ./categorize me me.csv ELV elv.csv the_rest.csv
int main(int argc, char *argv[]){//可使用命令行参数 
    if(argc!=6){
    	fprintf(stderr,"需要5个参数 5 arguments \n");
    	return 1;
    } 
    char line[80];//字符数组
    
	FILE *in;//创建 文件数据流 指针 in
	if( !(in = fopen("spooky.csv","r"))){
		fprintf(stderr,"文件不存在 file not existance \n");
		return 1;
	}
	
	FILE *out_file1 = fopen(argv[2],"w");          // 写方式打开 
    FILE *out_file2 = fopen(argv[4],"w");// 写方式打开 
	FILE *out_file3 = fopen(argv[5],"w");         // 写方式打开 
	while(fscanf(in,"%79[^\n]", line)==1){//成功读取一行 数据 存入 line中 
		if(strstr(line, argv[1]))
			fprintf(out_file1, "%s\n",line); 
		if(strstr(line, argv[3]))
			fprintf(out_file2, "%s\n",line);			
		else
			fprintf(out_file3, "%s\n",line);
					
	}
	fclose(out_file1);//flow closed  关闭文件数据流 
	fclose(out_file2);
	fclose(out_file3);
	
	return 0;
} 
