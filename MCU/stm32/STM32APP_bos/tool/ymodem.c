#include 	 "ymodem.h"

/*
SENDER:发送方。

RECEIVER:接收方。

第一步先由接收方，发送一个字符'C'

发送方收到'C'后，发送第一帧数据包，内容如下：

SOH 00 FF Foo.c NUL[123] CRC CRC

第1字节SOH:表示本包数据区大小有128字节。如果头为STX表示本包数据区大小为1024

第2字节00: 编号，第一包为00,第二包为01，第三包为02依次累加。到FF后继续从0循环递增。

第3字节FF: 编号的反码。 编号为00 对应FF，为01对应FE，以此类推。

 

第4字节到最后两字节：若第1字节为SOH时有128字节，为STX时有1024字节，这部分为数据区。
“Foo.c” 文件名， 超级终端下，在文件名后还有文件大小。
官方dome也是因为使用了这个文件大小进行比对。
这就是为什么用SecureCRT中的YMODEM协议而无法正确传输的原因。

         在文件名和文件大小之后，如果不满128字节，以0补满。

最后两字节：这里需要注意，只有数据部分参与了效CRC验,不包括头和编码部分。

16位CRC效验，高字节在前，低字节在后。

 
接收方收到第一帧数据包后，发送ACK正确应答。

然后再发送一个字符'C'。

发送方收到'C'后，开始发送第二帧，第二帧中的数据存放的是第一包数据。

接收方收到数据后，发送一个ACK然后等待下一包数据传送完毕，继续ACK应答。直到所有数据传输完毕。

数据传输完毕后，发送方发EOT，第一次接收方以NAK应答，进行二次确认。

发送方收到NAK后，重发EOT，接收方第二次收到结束符，就以ACK应答。

最后接收方再发送一个'C'，发送方在没有第二个文件要传输的情况下，

发送如下数据

SOH 00 FF 00~00(共128个) CRCH CRCL

接收方应答ACK后，正式结束数据传输。
*/

int fd;
char CrcFlag=0;
unsigned int FileLen=0;
unsigned int FileLenBkup=0;

#ifndef WINYWY
int main(int argc,char* argv[])
{

	int i;
	char bt;
	char* fp;
	int trans_ind;
	int set_ind;
	char* device;

	device="/dev/ttyUSB0";//serial port 2
	
	set_ind=set_com_common(device);//配置端口
	if(set_ind)
	{
		if(set_ind==COM_OPEN_ERR)
			printf("sorry,I can't open the serial port!\n");
		else
			printf("sorry,error ocurred when set the searial port!\n");
		exit(1);
	}
	switch(*argv[1])
	{
		case 'r'://接收文件
			trans_ind=control_recv(device);
			break;
		case 's'://发送文件
			fp=argv[2];
			if(fp==NULL)
			{
				printf("please input file name!\n");
				exit(1);
			}
			trans_ind=control_send(fp,device);
			break;
		default :
			printf("please use 'r' or 's fname' cmd to receive or send file\n");
			return 0;
	}

	if(trans_ind)
	{
		printf("sorry! get error in transmission!ERR ID:0x%x\n",trans_ind);
		if(trans_ind==COM_OPEN_ERR)
		{
			printf("sorry,I can't open the serial port!\n");
		}
		else
		{
			if((fd=_open(device, _O_RDWR|_O_BINARY|_O_NONBLOCK ))==-1)
			{
				printf("sorry,I can't open the serial port!\n");
			}
			else
			{
				bt=(char)CAN;
				if(_write(fd, &bt,1)==-1) //request sender to send file with CRC check
				{
					_close(fd);
					exit(1);
				}
				if(_write(fd, &bt,1)==-1) //request sender to send file with CRC check
				{
					_close(fd);
					exit(1);
				}
				delay(10000000);
			}
			_close(fd);
			exit(1);
		}
		
	}
	return 0;

}

#else
int main()
{
	char* fp;
	int trans_ind;
	char* device;

	device="COM1";
	fp="receive.c";

	set_com_common(device);
	trans_ind=control_send(fp,device);
	
	if(trans_ind)
	{
		printf("trans_ind:0x%x\n",trans_ind);
		printf("sorry! get error in transmission\n");
	}
	return 0;
}
#endif

int set_com_common(char* device) //common for receive and send
{
	
#ifndef WINYWY
// http://blog.csdn.net/specialshoot/article/details/50707965
	struct termios Opt;

	if((fd=open(device, O_RDWR))==-1)//打开端口　读写　
		return(COM_OPEN_ERR);//打开端口错误
	
	tcgetattr(fd, &Opt);
	cfsetispeed(&Opt,B115200);//波特率115200Bps
        Opt.c_lflag &= ~ICANON;
	Opt.c_lflag &= ~IEXTEN;
	Opt.c_lflag &= ~ISIG;
	Opt.c_lflag &= ~ECHO;

	Opt.c_iflag = 0;
	
	Opt.c_oflag = 0;
        //控制模式标志，指定终端硬件控制信息
        Opt.c_cflag |= CS8;//8个数据位
        Opt.c_cflag &= ~PARENB;//无奇偶校验
        Opt.c_cflag &=  ~CSTOPB;//1个停止位
        Opt.c_cflag &=  ~CRTSCTS;//关闭数据流控
        /*激活新配置*/   
	if (tcsetattr(fd,TCSANOW,&Opt) != 0)   
	{ 
		return (COM_SET_ERR);  
	} 
	close(fd);
#endif
	
	return 0;
}


void delay(int clock_count)
{
	int i;
	for(i=0;i<clock_count;i++)  //delay
	{
	}
}

