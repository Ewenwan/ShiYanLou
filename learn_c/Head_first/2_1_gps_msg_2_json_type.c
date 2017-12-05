#include<stdio.h>
// ./gps < gps.csv > gps.json 数据流重定向
// 2> reeor.txt  重定向错误输出 
// gps.csv
// 42.3432222222221,-71.21344444444,Speed = 21
// gps.json
// data=[ 
//{latitude: 42.3432222222221, longitude: -71.21344444444, info: 'Speed = 21'},
// ]
int main(){
	float latitude;//纬度
	float longitude;//经度 
	char info[80]; //速度信息字符数组
	int started=0;// ,换行标识开始
	
	puts("data,=[");
	while (scanf("%f,%f,%79[^\n]",&latitude,&longitude,info)==3){
	// %79[^\n]相当于说 把这一行余下的字符都给我 scanf() 返回成功读取的数据条数 
	if(started)	
	    printf(",\n");
	else
	    started=1;
	
	//  printf('我喜欢") 输出到标准输出  等同于 fprintf(stdout, "我喜欢");    类似 scanf();   和 fscanf(stdin,...); 
	// 前一个 f 代表 流 flow  后一个 f  代表 format 格式化 
	if((latitude>90)||(latitude<-90)){
	 //	printf("error"); //会输出到标准输出流  会和正确信息混杂在一起 
	    fprintf(stderr,"error"); 
		return 2;
	}
	if((longitude>180)||(longitude<-180)){
	 //	printf("error");
	    fprintf(stderr,"error"); 
		return 2;
	}    
	printf("{latitude: %f, longitude: %f, info: '%s'}",latitude,longitude,info);
	} 
	puts("\n}");
	
	return 0;
} 
