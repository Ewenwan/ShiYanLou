#include     "ymodem.h"

/***************************************************************************/
unsigned int count_read=0;          /*the bytes of datas received from buffer. count from every packet begin*/
unsigned char PacketStartFlagRecv=1;/*indicate one packet begin*/
unsigned int PacketLen=0;
unsigned char blknum_cur=0;

char rxdata[1029];                   /* An 8-bit integerthat is not signed.*/
char first_flag=1;                   /*it should be set to 1 every time when a file ready to transfer*/
FILE* fp_recv=NULL;
char* filename;

char filename_change=0;		
unsigned int BlockLength;
unsigned int RealLen=0;    /*FileLen:record the file length left if this field in the first block*/
						   /*ReaLen: the real block length barring pads in the last block*/

char* get_name(unsigned char* rec_buf);
void get_length(unsigned int name_len,unsigned char* rec_buf);

int procheader(unsigned char* rec_buf);
int procrecdata(unsigned char* rec_buf);


int control_recv(char* device) 
{
	int head_check;
	int file_check;
	int i;	
	int len;
	int CAN_CNT=0;
	char bt;
	char test=1;
	
	bt='C';
	CrcFlag=1;   //bt="C":meant CRC check
	
	if((fd=_open(device, _O_RDWR|_O_BINARY|_O_NONBLOCK ))==-1)
		return(COM_OPEN_ERR);

	count_read=0;
	for(;;)
	{

		delay(10000000);

		if(_write(fd, &bt,1)==-1) //request sender to send file with CRC check
		{
			printf("write err!\n");
			_close(fd);
			return(COM_WRITE_ERR);
		}

		for(i=0;i<0x1fffff;i++)
			delay(0xffffffff);


		if((len=_read(fd,rxdata+count_read,1))==-1)
		{
			if(errno==EAGAIN)
			{
				if(test)
				{
					test=0;
					printf("waiting...............\n");
				}
				continue;
			}
			_close(fd);
			return(COM_READ_ERR);
		}

		if(len)
		{
#ifdef DEBUG
			printf("%x\n",rxdata[0]);
#endif
			count_read+=len;
			break;
		}

	}

	while(1)
	{		
		if((len=_read(fd,rxdata+count_read,1))==-1)
		{
			if(errno==EAGAIN)
			{
				continue;
			}
			printf("read fail!we have read %d bytes\n",count_read);

			return(COM_READ_ERR);
		}
		else
		{
			count_read+=len;
		}
		
		if(count_read)
		{
			if(PacketStartFlagRecv)
			{	
				PacketStartFlagRecv=0;
				switch(rxdata[0])
				{
					case SOH:
						if(CAN_CNT)
							CAN_CNT=0;
#ifdef DEBUG
						printf("SOH!128 bytes per packet!\n");
#endif
						PacketLen=133;	//CRC check :128+5
						BlockLength=128;
						break;
					case STX:
						if(CAN_CNT)
							CAN_CNT=0;
#ifdef DEBUG
						printf("STX!1024 bytes per packet!\n");
#endif
						PacketLen=1029; //case CRC check
						BlockLength=1024;
						break;
					case EOT:
					{
						if(CAN_CNT)
							CAN_CNT=0;
#ifdef DEBUG
						printf("get EOT!\n");
#endif

						printf("file has been completly transfered!\n");
						if(filename_change)
						{
							filename_change=0;
							printf("File existed! Save it as %s \n",filename);
						}
						printf("File length: %dbytes\n",FileLenBkup);
						
						
						if(filename!=NULL)
						{
							free(filename);
							filename=NULL; //*filename=(char)NULL
						}

						RealLen=0;
						FileLen=0;
						PacketLen=0;
						count_read=0;
						blknum_cur=0;
						PacketStartFlagRecv=1;
						first_flag=1;		//wait for next file
						bt=(char)ACK;  // "ACK":config the file end
						if(_write(fd,&bt,1)==-1)
						{
							_close(fd);
							return(COM_WRITE_ERR);
						}
						bt='C';  //request continue transmission with CRC check
						if(_write(fd, &bt,1)==-1) //request sender to send file with CRC check
						{
							_close(fd);
							return(COM_WRITE_ERR);
						}
				    	
						break;

					}
					case CAN:
						CAN_CNT++;
						count_read=0;
						PacketStartFlagRecv=1;
						break;
					default:
					{
						if(CAN_CNT)
							CAN_CNT=0;
						printf("unvalid packet head,retry!the first data is:0x%x\n",rxdata[0]);
						PacketStartFlagRecv=1;
						count_read=0;
						bt=(char)NAK;  // "NAK":indicate retransmission
						if(_write(fd, &bt,1)==-1) //request sender to send file with CRC check
						{
							printf("write fail!\n");
							_close(fd);
							return(COM_WRITE_ERR);
						}			
					}
				}
			}
		}
		if(CAN_CNT==2)
		{
			printf("transmittion is canceled by user!\n");
			if(filename!=NULL) // if(*filename!=(char)NULL)
				remove(filename);
			_close(fd);
			exit(0);
		}
		if(!count_read)	//get EOT or unvilid packet head or CAN
			continue;

		if(count_read==PacketLen)
		{
#ifdef DEBUG			
			printf("get a packet completly!count_read:%d\n",count_read);
#endif
			PacketStartFlagRecv=1;
			count_read=0;

			head_check=procheader(rxdata);
	    	
			switch(head_check)
			{
				case HEAD_OK:
#ifdef DEBUG
					printf("head check OK!\n");
#endif
					file_check=procrecdata(rxdata);
					
					switch(file_check)
					{
						case FILE_OPEN_ERR:
#ifdef DEBUG
							printf("ERROR occured when open the file!\n");
#endif
							if(first_flag)
							{
								bt=(char)NAK; //filename packet .retrans if error occured
								if(_write(fd, &bt,1)==-1) //request the sender to send file with CRC check
								{
									_close(fd);
									return(COM_WRITE_ERR);
								}
								
							}
							else
							{
								bt=(char)CAN;
								if(_write(fd, &bt,1)==-1) //request sender to send file with CRC check
								{
									_close(fd);
									return(COM_WRITE_ERR);
								}
								if(_write(fd, &bt,1)==-1) //request sender to send file with CRC check
								{
									_close(fd);
									return(COM_WRITE_ERR);
								}
								delay(10000000);
								_close(fd);
								exit(0);
							}
						case NO_MORE_FILE:
#ifdef DEBUG
							printf("Files have been transfered completly\n");
#endif
							bt=(char)ACK;  //加入发送编辑框对应字符串 "ACK";确认结束
							if(_write(fd, &bt,1)==-1) //request sender to send file with CRC check
							{
								_close(fd);
								return(COM_WRITE_ERR);
							}
							delay(10000000);
							_close(fd);
							exit(0);
						default:
#ifdef DEBUG
							printf("continue next packet.....\n");
#endif
							bt=(char)ACK;  //加入发送编辑框对应字符串 "ACK",指示继续
							if(_write(fd, &bt,1)==-1) //request sender to send file with CRC check
							{
								_close(fd);
								return(COM_WRITE_ERR);
							}

							if(first_flag)
							{
								bt='C';  //加入发送编辑框对应字符串 "C";指示以CRC检测的方式发送文件内容
								if(_write(fd, &bt,1)==-1) //request sender to send file with CRC check
								{
									_close(fd);
									return(COM_WRITE_ERR);
								}
							}
				
					}
					break;
				case FORMAT_UNSUPPORT:
#ifdef DEBUG
					printf("ERROR! Unsupported Format!\n");
#endif
					if(first_flag)
					{
						bt=(char)NAK; //文件名数据包，出错指示重发
						if(_write(fd, &bt,1)==-1) //request sender to send file with CRC check
						{
							_close(fd);
							return(COM_WRITE_ERR);
						}
					
					}
					else
					{
						bt=(char)CAN;  //加入发送编辑框对应字符串 "CAN";指示结束
						if(_write(fd, &bt,1)==-1) //request sender to send file with CRC check
						{
							_close(fd);
							return(COM_WRITE_ERR);
						}
						if(_write(fd, &bt,1)==-1) //request sender to send file with CRC check
						{
							_close(fd);
							return(COM_WRITE_ERR);
						}
						_close(fd);
						exit(FORMAT_UNSUPPORT);
					}
					break;
				case CRC_ERR:
#ifdef DEBUG
					printf("CRC ERROR!\n");
#endif
				case BLK_REPEAT:
#ifdef DEBUG
					printf("ERROR! Blocks repeat!\n");
#endif
					bt=(char)NAK;  //加入发送编辑框对应字符串 "NAK";指示重发
					if(_write(fd, &bt,1)==-1) //request sender to send file with CRC check
					{
						_close(fd);
						return(COM_WRITE_ERR);
					}
				
					break;
				case BLK_LOSS:
#ifdef DEBUG
					printf("ERROR! Blocks lost!\n");
#endif
					if(first_flag)
					{
						bt=(char)NAK; //文件名数据包，出错指示重发
						if(_write(fd, &bt,1)==-1) //request sender to send file with CRC check
						{
							_close(fd);
							return(COM_WRITE_ERR);
						}
//						delay(10000000);
					}
					else
					{
						bt=(char)CAN;  //加入发送编辑框对应字符串 "CAN";指示结束
						if(_write(fd, &bt,1)==-1) //request sender to send file with CRC check
						{
							_close(fd);
							return(COM_WRITE_ERR);
						}
						if(_write(fd, &bt,1)==-1) //request sender to send file with CRC check
						{
							_close(fd);
							return(COM_WRITE_ERR);
						}
						_close(fd);
						exit(-1);
					}
			}
		}

	}
}

/*************************************************************************/
/*************************************************************************/
int procheader(unsigned char* rec_buf)
{
	unsigned int i;
	unsigned short oldcrc=0,checksum=0;

	unsigned char blknum;
	
	/*check blocknum*/
	blknum=rec_buf[1];

	if(blknum!=blknum_cur)
	{
		if(blknum==blknum_cur-1)
		{
			blknum_cur-=1;
			return(BLK_REPEAT);
		}
		else
		{
			return(BLK_LOSS);
		}
	}
	
	blknum_cur=blknum+1;
	
	/*check crc*/
	if(CrcFlag)
	{

		for(i=3;i<BlockLength+5;i++)
		{
			oldcrc=updcrc(rec_buf[i], oldcrc);
		}
		
		if (oldcrc & 0xFFFF)
		{
			blknum_cur-=1;
			return(CRC_ERR);
		}

	}
	else
	{
	/**********************reserved for checksum Pro******************************/
		for(i=3;i<BlockLength+3;i++)
		{
			checksum+=rec_buf[i];
		}
		if((checksum-rec_buf[i])&0377) 
		{
			return(CHECKSUM_ERR);
		}
	}

	return(HEAD_OK);
}

/***************************************************************/
/***************get block;the first block has filename**********/
/***************************************************************/
int procrecdata(unsigned char* rec_buf)
{
//	int percent_new;


	if(first_flag&&(rec_buf[1]==0))   /*the first block (if num_blocks>256,needed first_flag)*/
	{
		
		filename=get_name(rec_buf);
		
		if(strlen(filename))
		{
			get_length(strlen(filename),rec_buf);  /*get FielLen*/

#ifndef WINYWY			
			while(access(filename,F_OK)==0)
			{
				strcat(filename,"~");
				if(!filename_change)
					filename_change=1;
			}
#endif

		}
		else
		{

			if(filename!=NULL)// (filename!=(char)NULL)
			{
				free(filename);
				filename=NULL;
			}

			return(NO_MORE_FILE);/*empty file name:sender indicate no more files*/
		}
	}
	else
	{
#ifdef DEBUG
		printf("blknum:%d\n",rec_buf[1]);
#endif
		
		if(first_flag)
		{
			first_flag=0;
		}

		if((fp_recv=fopen(filename,"ab"))==NULL)
		{
			return(FILE_OPEN_ERR);
		}
		
		if(FileLen&&(FileLen<BlockLength))
		{
			RealLen=FileLen;
			FileLen=0;
		}
		else
		{
			RealLen=BlockLength;
			FileLen-=BlockLength;
		}
    	
		fwrite(rec_buf+3,RealLen,1,fp_recv);/*put the datas in file*/

		fclose(fp_recv);
	}
	return 0;
}

char* get_name(unsigned char* rec_buf)
{
	unsigned int i;
	char* file_name=(char*)malloc(BlockLength);
	
	memset(file_name,'\0',BlockLength);
	
	for(i=3;i<BlockLength+3;i++)
	{
		if(rec_buf[i]=='\0')
			break;
		else
			file_name[i-3]=rec_buf[i];
	}
	
	return file_name;
}

void get_length(unsigned int name_len,unsigned char* rec_buf)
{
	unsigned short i;
	unsigned char count_len=0;
	unsigned int pos_len;
	
	pos_len=name_len+4;

	for(i=pos_len;;i++)
	{
		if((rec_buf[i]!=0x20)&(rec_buf[i]!='\0'))/*space and null*/

			count_len++;
		else
			break;
	}
	for(i=pos_len;count_len>0;i++)
	{

		FileLen+=(unsigned int)((rec_buf[i]&0x0f)*pow(10,--count_len));

	}
	FileLenBkup=FileLen;
	return;
}
