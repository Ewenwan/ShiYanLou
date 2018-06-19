/*******************************************************************************
** 文件名: 		download.c
** 版本：  		1.0
** 工作环境: 	RealView MDK-ARM 5 
** 作者: 		wuguoyana
** 生成日期: 	2011-04-29
** 功能:		等待用户选择传送文件操作,或者放弃操作以及一些提示信息，
                但真正实现传送的是ymodem．c源文件。
** 相关文件:	common.h
** 修改日志：	2011-04-29   创建文档
*******************************************************************************/

/* 包含头文件 *****************************************************************/
#include "common.h"

/* 变量声明 ------------------------------------------------------------------*/
extern uint8_t file_name[FILE_NAME_LENGTH];
uint8_t tab_1024[1024] =
{
    0
};

/*******************************************************************************
  * @函数名称	SerialDownload
  * @函数说明   通过串口接收一个文件
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void SerialDownload(void)
{
    uint8_t Number[10] = "          ";// 存储文件大小的字符串变量
    int32_t Size = 0;//文件大小
    // 提示信息
    SerialPutString("Waiting for the file to be sent ... (press 'a' to abort)\n\r");
    Size = Ymodem_Receive(&tab_1024[0]);//接收文件 并返回文件大小
    if (Size > 0) //文件下载成功  这里不知道 程序是否完整 ？？
    {
        SerialPutString("\n\n\r Programming Completed Successfully!\n\r--------------------------------\r\n Name: ");
        SerialPutString(file_name);//文件名
        Int2Str(Number, Size);     //整数转换成字符串  数字Size 转化成字符串Number
        SerialPutString("\n\r Size: ");
        SerialPutString(Number);   //显示大小
        SerialPutString(" Bytes\r\n");
		    // SerialPutString(" Bytes\r\n");
        SerialPutString("-------------------\n");
			  FLASH_WriteByte(g_bUpdateFlag_Address,0x00); //清理 升级标志
			  NVIC_SystemReset();  //更新完成后重启系统
    }
    else if (Size == -1)//文件大小超过可用内存空间
    {
        SerialPutString("\n\n\rThe image size is higher than the allowed space memory!\n\r");
    }
    else if (Size == -2)//文件CRC校验失败
    {
        SerialPutString("\n\n\rVerification failed!\n\r");
    }
    else if (Size == -3)//用户终止
    {
        SerialPutString("\r\n\nAborted by user.\n\r");
    }
    else                //接收文件失败
    {
        SerialPutString("\n\rFailed to receive the file!\n\r");
    }
		
}

/*******************************文件结束***************************************/
