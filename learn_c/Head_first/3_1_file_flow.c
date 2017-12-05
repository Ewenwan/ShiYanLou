#include<stdio.h>
#include<stdlib.h>
#include<string.h>
 
// gcc file_flow.c -o categorize && ./categorize
int main(){
    char line[80];//字符数组
	FILE *in = fopen("spooky.csv","r");//只读方式打开文件 创建 文件数据流 指针 in
	FILE *out_file1 = fopen("ufos.csv","w");          // 写方式打开 
    FILE *out_file2 = fopen("disappearances.csv","w");// 写方式打开 
	FILE *out_file3 = fopen("other.csv","w");         // 写方式打开 
	while(fscanf(in,"%79[^\n]", line)==1){//成功读取一行 数据 存入 line中 
		if(strstr(line, "UFO"))
			fprintf(out_file1, "%s\n",line); 
		if(strstr(line, "Disappearances"))
			fprintf(out_file2, "%s\n",line);			
		else
			fprintf(out_file3, "%s\n",line);
					
	}
	fclose(out_file1);//flow closed  关闭文件数据流 
	fclose(out_file2);
	fclose(out_file3);
	
	return 0;
} 
