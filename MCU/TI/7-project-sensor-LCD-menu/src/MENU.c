/*
 *四级菜单头文件
 *by万有文
 *
 */
  #include "MSP430G2553.h"
  #include "LCD12864.h"
  #include "DHT11.h"
  #include "MQxilie.h"


typedef unsigned char u8;                              //数据类型定义
typedef unsigned int  u16;
typedef unsigned long u32;

 u16 T_set=30,M_set=80,CO_set=30,CH4_set=80,jq_set=10,ben_set=15,pm_set=55;
 u8 pm_dj=4,baoj=1;


void welcom_m( void )
{

	delay(10);
    lcdinit();
    W_str(0,1,(unsigned char*)"室内环境监控装置");
	W_str(1,2,(unsigned char*)"上海海事大学");
    W_str(1,3,(unsigned char*)"物流工程学院");
    W_str(2,4,(unsigned char*)"测控122");
 //   P2IFG=0;
	}


void gund_1( void )
{

	 delay(10);
     DHT11_UART_init();           //DHT11串行通信初始化
	 DHT11_port_init();           //DHT11链接端口初始化
     lcdinit();
     W_str(1,1,(unsigned char*)"温度：");
	 W_str(5,1,(unsigned char*)"℃");
     W_str(1,2,(unsigned char*)"湿度：");
	 W_str(5,2,(unsigned char*)"%");
	 W_str(0,3,(unsigned char*)"温度报警值:");
	 //却少报警值的录入
	 write_com(0x8E);
	 write_figurer(T_set/10);
	 write_figurer(T_set%10);
     W_str(0,4,(unsigned char*)"湿度报警值:");
	 write_com(0x9E);
	 write_figurer(M_set/10);
	 write_figurer(M_set%10);
    // _enable_interrupts();

	 start_DHT11();
	 DHT11_printf();

  // P2IFG=0;
}

void gund_2( void )
{
  delay(10);
  lcdinit();
  W_str(0,1,(unsigned char*)"CO:");
  W_str(6,1,(unsigned char*)"ppm");
  W_str(0,2,(unsigned char*)"CH4:");
  W_str(6,2,(unsigned char*)"ppm");
  W_str(0,3,(unsigned char*)"一碳报警值:");
	 write_com(0x8E);
	 write_figurer(CO_set/10);
	 write_figurer(CO_set%10);

  W_str(0,4,(unsigned char*)"甲烷报警值:");
	 write_com(0x9E);
	 write_figurer(CH4_set/10);
	 write_figurer(CH4_set%10);


	   MQ1_adc10_init();       //MQ2_adc10初始化
	   MQ1_Vol_Con();          //数据转换
	   MQ1_printf();           //只显示甲烷和CO
}

// 在空气质量菜单 行标志为2  且按下确认键时   执行
void gund_3( void )
{
  delay(10);
  lcdinit();
  W_str(0,1,(unsigned char*)"甲醛:");
  W_str(6,1,(unsigned char*)"ppm");
  W_str(0,2,(unsigned char*)"苯:");
  W_str(6,2,(unsigned char*)"ppm");
  W_str(0,3,(unsigned char*)"甲醛报警值:");
	 write_com(0x8E);
	 write_figurer(jq_set/10);
	 write_figurer(jq_set%10);
  W_str(0,4,(unsigned char*)"苯报警值:");
	 write_com(0x9E);
	 write_figurer(ben_set/10);
	 write_figurer(ben_set%10);

	   MQ2_adc10_init();       //MQ2_adc10初始化
	   MQ2_Vol_Con();          //数据转换
	   MQ2_printf();           //只显示甲烷和CO

}

// 在空气质量菜单 行标志为3 且按下确认键时   执行
void gund_4( void )
{
	  delay(10);
	  lcdinit();
	  // W_byte(0,1,0x30);说明，这个是说，LCD 显示只认识ASCII码
	  W_str(1,1,(unsigned char*)"PM:");
	  W_str(5,1,(unsigned char*)"ug/m3");
	 // W_byte(0,0,0x88);//可以用这种方式写指令
	  //  W_str(1,2,"22:52");
	   W_str(1,2,(unsigned char*)"等级:");
		 switch(pm_dj)
		 {
		 case 1:W_str(5,2,(unsigned char*)"差");break;
		 case 2:W_str(5,2,(unsigned char*)"中");break;
		 case 3:W_str(5,2,(unsigned char*)"良");break;
		 case 4:W_str(5,2,(unsigned char*)"优");break;
		 default:W_str(5,2,(unsigned char*)"优");break;
		 }

	     W_str(0,3,(unsigned char*)"报警值:");
		 write_com(0x8C);
		 write_figurer(pm_set/10);
		 write_figurer(pm_set%10);
		  W_str(5,3,(unsigned char*)"ug/m3");

		  PM_adc10_init();
		  PM_Vol_Con();
		  PM_printf();
	//       PM2.5数据测量和显示模块
}


// 在空气质量菜单 行标志为4 且按下确认键时   执行



// 在主菜单页面 行标志为3且按下确认键时   执行
void manu_1( void )
{

	delay(10);
    lcdinit();
    W_str(1,1,(unsigned char*)"报警参数设定");
	W_str(2,2,(unsigned char*)"报警开关");
    W_str(1,3,(unsigned char*)"返回滚动界面");
	}

// 在参数设定菜单 行标志为1（默认）且按下确认键时   执行
void manu_211( void )
{

	delay(10);
    lcdinit();
    W_str(1,1,(unsigned char*)"温度报警设定");
	W_str(1,2,(unsigned char*)"湿度报警设定");
    W_str(1,3,(unsigned char*)"CO报警值设定");
    W_str(1,4,(unsigned char*)"甲烷报警值设定");
	}
void manu_212( void )
{

	delay(10);
    lcdinit();
    W_str(1,1,(unsigned char*)"甲醛报警值设定");
	W_str(1,2,(unsigned char*)"苯报警设定");
    W_str(1,3,(unsigned char*)"粉尘报警值设定");
    W_str(1,4,(unsigned char*)"返回");
	}


//湿度报警值设定(默认50%)    按上下键后每次加减5
void manu_22( void )
{
	delay(10);
	    lcdinit();
	    W_str(1,1,(unsigned char*)"报警状态:");
	    switch(baoj)
	    		 {
	    		 case 0:W_str(6,1,(unsigned char*)"关");break;
	    		 case 1:W_str(6,1,(unsigned char*)"开");break;
	    		 default:W_str(6,1,(unsigned char*)"开");
	    		 }
		W_str(1,2,(unsigned char*)"关闭");
	    W_str(1,3,(unsigned char*)"打开");
	    W_str(1,4,(unsigned char*)"返回");

	}


//甲醛报警值设定(默认50ppm)    按上下键后每次加减5
void manu_31( void )
{
	delay(10);
    lcdinit();
    W_str(1,1,(unsigned char*)"温度报警值:");
    write_com(0x86);
   	write_figurer(T_set/10);
   	write_figurer(T_set%10);
    W_str(1,2,(unsigned char*)"增加");
    W_str(1,3,(unsigned char*)"减少");
    W_str(1,4,(unsigned char*)"返回");
//差   报警值显示
	}

void manu_32( void )
{
	delay(10);
    lcdinit();
    W_str(1,1,(unsigned char*)"湿度报警值:");
    write_com(0x86);
   	write_figurer(M_set/10);
   	write_figurer(M_set%10);
    W_str(1,2,(unsigned char*)"增加");
    W_str(1,3,(unsigned char*)"减少");
    W_str(1,4,(unsigned char*)"返回");
//差   报警值显示
	}

void manu_33( void )
{
	delay(10);
    lcdinit();
    W_str(1,1,(unsigned char*)"CO报警值:");
    write_com(0x86);
   	write_figurer(CO_set/10);
   	write_figurer(CO_set%10);
    W_str(1,2,(unsigned char*)"增加");
    W_str(1,3,(unsigned char*)"减少");
    W_str(1,4,(unsigned char*)"返回");
//差   报警值显示
	}

void manu_34( void )
{
	delay(10);
    lcdinit();
    W_str(1,1,(unsigned char*)"甲烷报警值:");
    write_com(0x86);
   	write_figurer(CH4_set/10);
   	write_figurer(CH4_set%10);
    W_str(1,2,(unsigned char*)"增加");
    W_str(1,3,(unsigned char*)"减少");
    W_str(1,4,(unsigned char*)"返回");
//差   报警值显示
	}

void manu_35( void )
{
	delay(10);
    lcdinit();
    W_str(1,1,(unsigned char*)"甲醛报警值:");
    write_com(0x86);
   	write_figurer(jq_set/10);
   	write_figurer(jq_set%10);
    W_str(1,2,(unsigned char*)"增加");
    W_str(1,3,(unsigned char*)"减少");
    W_str(1,4,(unsigned char*)"返回");
//差   报警值显示
	}

void manu_36( void )
{
	delay(10);
    lcdinit();
    W_str(1,1,(unsigned char*)"苯报警值:");
    write_com(0x86);
   	write_figurer(ben_set/10);
   	write_figurer(ben_set%10);
    W_str(1,2,(unsigned char*)"增加");
    W_str(1,3,(unsigned char*)"减少");
    W_str(1,4,(unsigned char*)"返回");
//差   报警值显示
	}

void manu_37( void )
{
	delay(10);
    lcdinit();
    W_str(0,1,(unsigned char*)"粉尘报警值:");
    write_com(0x86);
   	write_figurer(pm_set/10);
   	write_figurer(pm_set%10);
    W_str(1,2,(unsigned char*)"增加");
    W_str(1,3,(unsigned char*)"减少");
    W_str(1,4,(unsigned char*)"返回");
//差   报警值显示
	}



//PM2.5报警值设定(默认50ug/m3)    按上下键后每次加减5
