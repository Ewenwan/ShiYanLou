#include<stdio.h>
// (./fliter | ./gps) < gps.csv > gps.json 数据流重定向
int main(){
	float latitude;//纬度
	float longitude;//经度 
	char info[80]; //速度信息字符数组	
	while (scanf("%f,%f,%79[^\n]",&latitude,&longitude,info)==3)
		if((latitude>34)&&(latitude<76))
			if((longitude>-76)&&(longitude<-64))
				printf("%f,%f,%s\n",latitude,longitude,info);// 把部分 符合要求的数据 输出到标准输出流 
	return 0;
} 
