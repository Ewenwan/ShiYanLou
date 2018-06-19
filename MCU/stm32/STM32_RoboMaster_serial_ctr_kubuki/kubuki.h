
#include "usart.h"	 
#include "control.h"
#include <string.h> 
#include <stdbool.h>

// 串口收到的字符

char chr;
extern char chr;
//kobuki接口解析
#define clearBuffer 0
#define waitingForStx 1
#define waitingForPayloadSize 2
#define waitingForPayloadToEtx 3
#define	waitingForEtx 4

float	disance_m = 0.4; //底盘两轮 轮距 0.4m

//koubki接口消息头
u8 STX[2];

//kobuki接口解析的状态
u8 cmd_state;
u8 size_length_field;
u8 size_payload;
bool found_package;
u8 identifier;
s16 motor_speed;
s16 motor_radius;
bool retController;
bool retExtData;
u8   extDataFlag;

//串口收到的字符Buffer  （Kobuki)
char buffer[50];  //用于仿kobuki

//only for test
char tempBuf[50]; 
char tempIndex;
int index_ = 0;


/* 发送基本数据给Firefly */
void sendBasicData()
{
	u8 cs;
	u8 i;

	//当前时间戳 0~65535循环
	u16 cur_time     =  ptForkLift->u16RunSendPeriodMotor;
	//左编码器累加值
	u16 encoder_left =  ptForkLift->u16EncoderSum;
	//右编码器累加值
	u16 encoder_right =  ptForkLift2->u16EncoderSum;
	
	//格式转换
	u8 sendBuf[21];
//#if 0	
	sendBuf[0] = 0xAA;//Header 0
	sendBuf[1] = 0x55;//Header 1
	sendBuf[2] = 0x11;//payload长度 = 16
	sendBuf[3] = 0x01;//payload id = 1
	sendBuf[4] = 0x0F;//payload length = 15
	sendBuf[5] = cur_time&0x00FF;//ayload 时间戳 低位
	sendBuf[6] = (cur_time&0xFF00)>>8 ;//payload 时间戳 高位
	sendBuf[7] = 0x00;//payload 减震器
	sendBuf[8] = 0x00;//payload 轮跌落
	sendBuf[9] = 0x00;//payload 悬崖跌落
	sendBuf[10] = encoder_left&0x00FF;//payload 左编码器累加值低位
	sendBuf[11] = (encoder_left&0xFF00)>>8;//payload 左编码器累加值高位
	sendBuf[12] = encoder_right&0x00FF;//payload 右编码器累加值低位
	sendBuf[13] = (encoder_right&0xFF00)>>8;//payload 右编码器累加值高位
	sendBuf[14] = 0x00;//payload PWM左
	sendBuf[15] = 0x00;//payload PWM右
	sendBuf[16] = 0x00;//payload button
	sendBuf[17] = 0x00;//payload charge
	sendBuf[18] = 0x00;//payload battery
	sendBuf[19] = 0x00;//payload 过流告警
//#endif

	//加上checksum校验位
	cs = 0x00;
	for (i = 2; i < 20; i++)
	{
	  cs ^= sendBuf[i];
	}
	sendBuf[20] = cs;	

//	printf("send %d  %d \n\r",sendBuf[10],sendBuf[11]);
	//输出串口	
	for (i = 0; i < 21; i++)
		printf("%c",sendBuf[i]);

}

void sendControllerData()
{
	//格式转换
	u8 sendBuf[19];
	u8 cs;
	u8 i;
	
	sendBuf[0] = 0xAA;//Header 0
	sendBuf[1] = 0x55;//Header 1
	sendBuf[2] = 0x0F;//payload长度 = 15
	sendBuf[3] = 0x15;//payload id = 21
	sendBuf[4] = 0x0D;//payload length = 13
	sendBuf[5] = 0x00;//pid_type
	sendBuf[6] = 0x00 ;//pid_p
	sendBuf[7] = 0x00 ;//pid_p
	sendBuf[8] = 0x00 ;//pid_p
	sendBuf[9] = 0x00 ;//pid_p
	sendBuf[10] = 0x00 ;//pid_i
	sendBuf[11] = 0x00 ;//pid_i
	sendBuf[12] = 0x00 ;//pid_i
	sendBuf[13] = 0x00 ;//pid_i
	sendBuf[14] = 0x00 ;//pid_d	
	sendBuf[15] = 0x00 ;//pid_d	
	sendBuf[16] = 0x00 ;//pid_d	
	sendBuf[17] = 0x00 ;//pid_d	

	//加上checksum校验位
	cs = 0x00;
	for (i = 2; i < 18; i++)
	{
	  cs ^= sendBuf[i];
	}
	sendBuf[18] = cs;	

	//输出串口	
	for (i = 0; i < 10; i++)
		printf("%c",sendBuf[i]);
	
}

void sendExtData()
{
	//格式转换
	u8 sendBuf[30];
	u8 cs;
	u8 i;

	//一起发过去得了（hardware firmware UniqueDeviceId)		
	
	sendBuf[0] = 0xAA;//Header 0
	sendBuf[1] = 0x55;//Header 1
	sendBuf[2] = 0x1a;//payload长度 = 26
	
	sendBuf[3] = 0x0a;//payload id = 10  ------ hardware version
	sendBuf[4] = 0x04;//payload length = 4
	sendBuf[5] = 0x00;//patch
	sendBuf[6] = 0x03 ;//minor
	sendBuf[7] = 0x01 ;//major
	sendBuf[8] = 0x00 ;//unused
	
	sendBuf[9] = 0x0b ;//payload id = 11 ---- firmware version
	sendBuf[10] = 0x04 ;//payload len = 4
	sendBuf[11] = 0x00 ;//patch
	sendBuf[12] = 0x03 ;//minor
	sendBuf[13] = 0x01 ;//major
	sendBuf[14] = 0x00 ;//unused	

	
	sendBuf[15] = 0x13 ;//payload id = 19 ---- UDID	
	sendBuf[16] = 0x0c ;//payload len
	sendBuf[17] = 0x00 ;//udid
	sendBuf[18] = 0x00 ;//udid
	sendBuf[19] = 0x00 ;//udid
	sendBuf[20] = 0x00 ;//udid
	sendBuf[21] = 0x00 ;//udid
	sendBuf[22] = 0x00 ;//udid
	sendBuf[23] = 0x00 ;//udid
	sendBuf[24] = 0x00 ;//udid
	sendBuf[25] = 0x00 ;//udid
	sendBuf[26] = 0x00 ;//udid
	sendBuf[27] = 0x00 ;//udid
	sendBuf[28] = 0x01 ;//udid

	//加上checksum校验位
	cs = 0x00;
	for (i = 2; i < 29; i++)
	{
	  cs ^= sendBuf[i];
	}
	sendBuf[29] = cs;	

	//输出串口	
	for (i = 0; i < 30; i++)
		printf("%c",sendBuf[i]);

}

bool WaitForStx(u8 datum)
{
  bool found_stx;
  u8 i;

  found_stx = true;

  // add incoming datum
  buffer[index_] = datum;
  index_++;

  // check whether we have STX
  for (i = 0; i < index_ && i < 2; i++)
  {
    if (buffer[i] != STX[i])
    {
      found_stx = false;
      index_ = 0;
      break;
    }
  }
  return (found_stx && index_ == 2);
}

bool waitForPayloadSize(u8 incoming)
{
  buffer[index_]= incoming;
  index_ ++;
  // check when we need to wait for etx
  if (index_ < 2 + size_length_field)  //如果buffer总长度 < (0xaa 0x55 + <payload siez> )
  {
    return false;
  }
  else
  {
    switch (size_length_field)
    {
      case 1: // kobuki
        size_payload = buffer[2];
        break;
      default:
        // put assertion failure
        size_payload = 1;
        break;
    }
    return true;
  }
}

bool waitForPayloadAndEtx(u8 incoming)
{
  buffer[index_] = incoming;
  index_++;

//  printf("waitForPayloadAndEtx index_:%d\r\n",index_);
  
  if ( size_payload > 10 ) { //如果长度太长了
    cmd_state = clearBuffer;
    return false;
  }
  // check when we need to wait for etx
  if (index_ < 2 + size_length_field + size_payload + 1)  
  {
    return false;
  }
  else
  {
//	  printf("found_package !!!!!!!!!!!!\r\n");
    found_package = true;

    return true;
  }
}


//用于仿kobuki接口的命令解析
bool update(u8 incoming)
{
	found_package = false;

	if ( cmd_state == clearBuffer ) {
	   memset(buffer, 0, sizeof(buffer));
	   index_ = 0;
	   found_package = false;
	   cmd_state = waitingForStx;
	 }
  	switch (cmd_state)
  	{
    case waitingForStx:
    	if (WaitForStx(incoming)==true)
      	{
          cmd_state = waitingForPayloadSize; // kobukibot
        }
        break;
    case waitingForPayloadSize:
      if (waitForPayloadSize(incoming)==true)
      {
        cmd_state = waitingForPayloadToEtx;
      }
      break;

    case waitingForPayloadToEtx:
      if (waitForPayloadAndEtx(incoming)==true)
      {
        cmd_state = clearBuffer;
      }
      break;

    default:
      cmd_state = waitingForStx;
      break;
  }

  if (found_package ==true) {
//  	printf("here return true");
    return true;	//what happen if checksum is equal to false(== -1)?
  } else {
    return false;
  }	
}

void runCommand_Motor(s16 speed, s16 radius)
{
	//表示直行
	if (radius==0)
	{
		 ptForkLift->s16x = speed / 1000.0;  //速度扩大了 1000倍
	   ptForkLift->s16y = 0;
	   ptForkLift->s16w = 0;	
	}
	else if (radius==1)
	{
		 ptForkLift->s16x = 0;  
	   ptForkLift->s16y = 0;
	   ptForkLift->s16w = speed*1.0  / (500 * disance_m);	// 自转角速度 500 * 轮距 上层 kubuki是0.23
	}
	else if (radius>0)
	{
		ptForkLift->s16y = 0;
		ptForkLift->s16w = speed*1.0/(radius + 500 * disance_m) ;
		ptForkLift->s16x = radius * ptForkLift->s16w / 1000.0 ;   
	}
	else
	{
		ptForkLift->s16y = 0;
		ptForkLift->s16w = speed*1.0/(radius - 500 * disance_m) ;
		ptForkLift->s16x = radius*1.0 * ptForkLift->s16w / 1000.0 ; 
	}
 //ptForkLift->u8Can_Msg_Send_flag=1;//置位  can下发标志
	 ptForkLift->u8Cmd_receive_lag=1;// 命令接收标志
   ptForkLift->u16SerialComCount[0]++;// 串口接收标志	
}



void kubuki_init(){
	memset(buffer, 0, sizeof(buffer)); //For kobuki,清空串口命令缓存
	STX[0] = 0xaa;
	STX[1] = 0x55;
	index_ = 0;
	size_length_field=1;
	cmd_state = clearBuffer;
	found_package = false;
	retController = false;
	retExtData = false;
}



