/*******************************************************************************
** ÎÄ¼şÃû: 		ymodem.c
** °æ±¾£º  		1.0
** ¹¤×÷»·¾³: 	RealView MDK-ARM 4.14
** ×÷Õß: 		wuguoyana
** Éú³ÉÈÕÆÚ: 	2011-04-29
** ¹¦ÄÜ:		ºÍYmodem.cµÄÏà¹ØµÄĞ­ÒéÎÄ¼ş
                ¸ºÔğ´Ó³¬¼¶ÖÕ¶Ë½ÓÊÕÊı¾İ(Ê¹ÓÃYmodemĞ­Òé)£¬²¢½«Êı¾İ¼ÓÔØµ½ÄÚ²¿RAMÖĞ¡£
                Èç¹û½ÓÊÕÊı¾İÕı³££¬Ôò½«Êı¾İ±à³Ìµ½FlashÖĞ£»Èç¹û·¢Éú´íÎó£¬ÔòÌáÊ¾³ö´í¡£
** Ïà¹ØÎÄ¼ş:	stm32f10x.h
** ĞŞ¸ÄÈÕÖ¾£º	2011-04-29   ´´½¨ÎÄµµ
*******************************************************************************/

/* °üº¬Í·ÎÄ¼ş *****************************************************************/

#include "common.h"
#include "stm32f10x_flash.h"

/* ±äÁ¿ÉùÃ÷ -----------------------------------------------------------------*/
uint8_t file_name[FILE_NAME_LENGTH];//½ÓÊÕµÄ¶ş½øÖÆÎÄ¼şÎÄ¼şµÄÎÄ¼şÃû
//ÓÃ»§³ÌĞòFlashÆ«ÒÆ
uint32_t FlashDestination = ApplicationAddress;//ÓÃ»§³ÌĞòÆğÊ¼ÏÂÔØ´æ´¢µØÖ·
uint16_t PageSize = PAGE_SIZE;//°ü´óĞ¡
uint32_t EraseCounter = 0x0;
uint32_t NbrOfPage = 0;       //flashÊÇ°´Ò³À´´æ´¢µÄ ÆğÊ¼µØÖ·µ½ÎÄ¼ş´óĞ¡ĞèÒªËùĞèµÄflashÒ³Êı
FLASH_Status FLASHStatus = FLASH_COMPLETE;
uint32_t RamSource;
extern uint8_t tab_1024[1024];

/*******************************************************************************
  * @º¯ÊıÃû³Æ	Receive_Byte
  * @º¯ÊıËµÃ÷   ´Ó·¢ËÍ¶Ë½ÓÊÕÒ»¸ö×Ö½Ú
  * @ÊäÈë²ÎÊı   c: ½ÓÊÕ×Ö·û
                timeout: ³¬Ê±Ê±¼ä
  * @Êä³ö²ÎÊı   ÎŞ
  * @·µ»Ø²ÎÊı   ½ÓÊÕµÄ½á¹û
                0£º³É¹¦½ÓÊÕ
                1£ºÊ±¼ä³¬Ê±
*******************************************************************************/
static  int32_t Receive_Byte (uint8_t *c, uint32_t timeout)
{
    while (timeout-- > 0)
    {
        if (SerialKeyPressed(c) == 1)//´®¿Ú½ÓÊÕÒ»¸ö×Ö½ÚÊı¾İ
        {
            return 0;
        }
    }
    return -1;
}

/*******************************************************************************
  * @º¯ÊıÃû³Æ	Send_Byte
  * @º¯ÊıËµÃ÷   ·¢ËÍÒ»¸ö×Ö·û
  * @ÊäÈë²ÎÊı   c: ·¢ËÍµÄ×Ö·û
  * @Êä³ö²ÎÊı   ÎŞ
  * @·µ»Ø²ÎÊı   ·¢ËÍµÄ½á¹û
                0£º³É¹¦·¢ËÍ
*******************************************************************************/
static uint32_t Send_Byte (uint8_t c)
{
    SerialPutChar(c);// ´®¿Ú·¢ËÍÒ»¸ö×Ö½Ú
    return 0;
}

/*******************************************************************************
  * @º¯ÊıÃû³Æ	Receive_Packet
  * @º¯ÊıËµÃ÷   ´Ó·¢ËÍ¶Ë½ÓÊÕÒ»¸öÊı¾İ°ü
  * @ÊäÈë²ÎÊı   data £º´æ´¢Êı¾İÖ¸Õë           Ê××Ö½Ú¶¨ÒåÊı¾İ°ü³¤¶È + Êı¾İ°üÍ·ĞÅÏ¢(5×Ö½Ú£¿) + 1024/128×Ö½ÚÊı¾İ
                length£º±¾°üÊı¾İ³¤¶È 128/1024
                timeout £º³¬Ê±Ê±¼ä
  * @Êä³ö²ÎÊı   ·µ»ØÊı¾İ°üµÄ´óĞ¡ 
  * @·µ»Ø²ÎÊı   ½ÓÊÕµÄ½á¹û
                0: Õı³£·µ»Ø
                -1: ³¬Ê±»òÕßÊı¾İ°ü´íÎó
                1: ÓÃ»§È¡Ïû
*******************************************************************************/
static int32_t Receive_Packet (uint8_t *data, int32_t *length, uint32_t timeout)
{
    uint16_t i, packet_size;
    uint8_t c; //½ÓÊÕµÄÒ»¸ö×Ö½ÚÊı¾İ
    *length = 0;

///////ÅĞ¶ÏµÚÒ»×Ö½ÚÊı¾İ////////////	
	//Ã¿¸öÊı¾İ°üµÄ µÚÒ»¸ö×Ö½ÚÊı¾İ Îª±êÖ¾ ´æÔÚcÖĞ
    if (Receive_Byte(&c, timeout) != 0)// ½ÓÊÕµÚÒ»¸ö×Ö½ÚÊı¾İ ÓÃ»§´«Êä´óĞ¡ ×´¿öµÈ  ´®¿Ú½ÓÊÕÒ»¸öÊı¾İ  RS485   CAN µÈÒ²¿ÉÒÔ
    {
        return -1;
    }
    switch (c)
    {
    case SOH:  // #define SOH   (0x01)  //128×Ö½ÚÊı¾İ°ü¿ªÊ¼
        packet_size = PACKET_SIZE;// Êı¾İ´«Êä¶Ë  ¶¨ÒåÊı¾İ°üµÄ´óĞ¡ 128
        break;
    case STX:  // #define STX   (0x02)  //1024×Ö½ÚµÄÊı¾İ°ü¿ªÊ¼
        packet_size = PACKET_1K_SIZE;// Êı¾İ°ü´óĞ¡Îª  1024
        break;
    case EOT:  // #define EOT                     (0x04)  //½áÊø´«Êä  end of transmit
        return 0;
    case CA:   // #define CA                      (0x18)  //ÕâÁ½¸öÏà¼ÌÖĞÖ¹×ªÒÆ cancel
        if ((Receive_Byte(&c, timeout) == 0) && (c == CA))
        {
            *length = -1;   //·¢ËÍ¶ËÖÕÖ¹    Á½¸ö  Á¬ĞøÁ½¸ö  CA=(0x18)  ±íÊ¾·¢¶ËÈ¡Ïû´«Êä
            return 0;     
        }
        else
        {
            return -1; 
        }
    case ABORT1:  // #define ABORT1   0x41)  //'A' == 0x41, ÓÃ»§ÖÕÖ¹ 
    case ABORT2:  // #define ABORT2   (0x61)  //'a' == 0x61, ÓÃ»§ÖÕÖ¹
        return 1;
    default:
        return -1;
    }
		
//////½ÓÊÕºóÃæµÄÊı¾İ/////////////////
    *data = c;// ´æ·ÅµÚÒ»¸ö×Ö½ÚÊı¾İ
		// °´ÕÕÊı¾İ°üµÚÒ»¸ö×Ö½Ú¶¨ÒåµÄ Êı¾İ°ü´óĞ¡½ÓÊÕÊı¾İ  ×¢Òâ»¹ÓĞÒ»¸öÊı¾İ°üÍ·ĞÅÏ¢
    for (i = 1; i < (packet_size + PACKET_OVERHEAD); i++)
    {
        if (Receive_Byte(data + i, timeout) != 0)//½ÓÊÕºóÃæµÄÊı¾İ  ´æÈëµ½ dataÄÚ
        {
            return -1;
        }
    }
    if (data[PACKET_SEQNO_INDEX] != ((data[PACKET_SEQNO_COMP_INDEX] ^ 0xff) & 0xff))
    {
        return -1;
    }
    *length = packet_size;//·µ»Ø½ÓÊÜµÄÊı¾İ°ü³¤¶È  128»òÕß 1024
    return 0;
}

/*******************************************************************************
  * @º¯ÊıÃû³Æ	Ymodem_Receive
  * @º¯ÊıËµÃ÷   Í¨¹ı ymodemĞ­Òé½ÓÊÕÒ»¸öÎÄ¼ş
  * @ÊäÈë²ÎÊı   ÎÄ¼ş´æ´¢Êı×é buf: Ê×µØÖ·Ö¸Õë
  * @Êä³ö²ÎÊı   ÎŞ
  * @·µ»Ø²ÎÊı   ÎÄ¼ş³¤¶È

·¢ËÍ·½:
µÈ´ı½ÓÊÕµ½ C  µÈ´ı·¢ËÍÎÄ¼ş±êÖ¾   
µÚ0¸öÊı¾İ°üÎª 3×Ö½Ú Êı¾İ°ü´óĞ¡±êÖ¾ + ±àºÅ00+±àºÅ·´ÂëFF  + 128×Ö½Ú/1024×Ö½Ú ÎÄ¼şÊôĞÔÊı¾İ°ü(ÎÄ¼şÃû×Ö  +  ÎÄ¼ş´óĞ¡ ²»×ã²¹Áã) + Á½×Ö½Ú CRCĞ£ÑéÂë
              µÈ´ı»ØÓ¦ACK
ÆäºóÎªÊı¾İ°ü  3×Ö½Ú Êı¾İ°ü´óĞ¡±êÖ¾ + ±àºÅ01+±àºÅ·´ÂëFE  + 128×Ö½Ú/1024×Ö½Ú ÄÚÈİ(²»×ã²¹³ä »»ĞĞ·û) + Á½×Ö½Ú CRCĞ£ÑéÂë
              µÈ´ı»ØÓ¦ACK
×îºó·¢ËÍ EOT½áÊø·¢ËÍ±êÖ¾
              µÈ´ı»ØÓ¦ACK
ÔÙ·¢ËÍÒ»°üÈ«ÎªÁãµÄÊı¾İ°ü 3×Ö½Ú Êı¾İ°ü´óĞ¡±êÖ¾ + ±àºÅ00+±àºÅ·´ÂëFF + 128×Ö½Ú 0Êı¾İ  + CRCĞ£ÑéÂë
              µÈ´ı»ØÓ¦ACK
×îºóÔÙÒ»´Î·¢ËÍ EOT½áÊø·¢ËÍ±êÖ¾£¬½áÊø·¢ËÍ
              µÈ´ı»ØÓ¦ACK

½ÓÊÕ·½£º
·¢ËÍ'C',µÈ´ı·¢ËÍ·¢ËÍ·½ÏìÓ¦
½ÓÊÕÊı¾İ°ü   µÚÒ»¸ö×ÖÎª ÎªÊı¾İ°ü´óĞ¡±êÖ¾  ¸ù¾İ±êÖ¾½ÓÊÕ×Ö·û²¢»ØÓ¦


*******************************************************************************/
int32_t Ymodem_Receive (uint8_t *buf)
{
	  // ¶¨ÒåÒ»¸öÊı¾İ°ü´óĞ¡  ÎÄ¼ş´óĞ¡
    uint8_t packet_data[PACKET_1K_SIZE + PACKET_OVERHEAD], file_size[FILE_SIZE_LENGTH], *file_ptr, *buf_ptr;
	// Êı¾İ°ü³¤¶È
    int32_t i, j, packet_length, session_done, file_done, packets_received, errors, session_begin, size = 0;

    //³õÊ¼»¯FlashµØÖ·±äÁ¿
    FlashDestination = ApplicationAddress;//ÓÃ»§³ÌĞòÏÂÔØµÄ³õÊ¼µØÖ·

    for (session_done = 0, errors = 0, session_begin = 0; ;)
    {
        for (packets_received = 0, file_done = 0, buf_ptr = buf; ;)
        {
					  // ½ÓÊÕÒ»¸öÊı¾İ°ü Êı¾İ´æ·ÅÔÚ packet_dataÄÚ  Êı¾İ°üÓĞĞ§Êı¾İ´óĞ¡ 128/1024 ´æ·ÅÔÚpacket_lengthÄÚ
            switch (Receive_Packet(packet_data, &packet_length, NAK_TIMEOUT))//½ÓÊÕÒ»¸öÊı¾İ°ü Êı¾İ°üÊ×µØÖ· ³¬Ê±
            {
            case 0://½ÓÊÕÊı¾İÕı³£
                errors = 0;
                switch (packet_length)//Êı¾İ°ü³¤¶È
                {
                    //·¢ËÍ¶ËÖÕÖ¹
                case - 1:
                    Send_Byte(ACK); // #define ACK                     (0x06)  //»ØÓ¦ÉÏÎ»»ú
                    return 0;
                    //½áÊø´«Êä
                case 0:
                    Send_Byte(ACK); // #define ACK                     (0x06)  //»ØÓ¦ÉÏÎ»»ú
                    file_done = 1;  // Êı¾İ½ÓÊÕÍê³É
                    break;
                    //Õı³£µÄÊı¾İ°ü
                default:
                    if ((packet_data[PACKET_SEQNO_INDEX] & 0xff) != (packets_received & 0xff))
                    {
                        Send_Byte(NAK); // #define NAK    (0x15)  //Ã»»ØÓ¦
                    }
                    else
                    {
///////////////////////µÚ0¸öÊı¾İ°ü   ÎªÎÄ¼şÃû+ÎÄ¼ş´óĞ¡Êı¾İ°ü/////////////////////////
                        if (packets_received == 0)
                        {
                            // ÎÄ¼şÃûÊı¾İ°ü
													  // ²Á³ıFLASH¿Õ¼ä  packet_data[PACKET_HEADER]Êı¾İÄÚÈİ¿ªÊ¼
                            if (packet_data[PACKET_HEADER] != 0)
                            {
                                //ÎÄ¼şÃûÊı¾İ°üÓĞĞ§Êı¾İÇøÓò  ´ÓPACKET_HEADERÏÂ±ê¿ªÊ¼ÎªÊı¾İ
															  //×¢Òâ·¢ËÍÎÄ¼şÃûºó ½ô½Ó×Å»áÓĞÒ»¸ö0 ÒÔ»®·Ö ÎÄ¼şÃûÇøÓòºÍ ÎÄ¼ş´óĞ¡ÇøÓò
                                for (i = 0, file_ptr = packet_data + PACKET_HEADER; (*file_ptr != 0) && (i < FILE_NAME_LENGTH);)
                                {
                                    file_name[i++] = *file_ptr++; // ÎÄ¼şÃûÇøÓò µ½ *file_ptr = 0 µÄµØ·½
                                }
                                file_name[i++] = '\0';//Ìí¼Ó½áÊø·û
                                for (i = 0, file_ptr ++; (*file_ptr != ' ') && (i < FILE_SIZE_LENGTH);)
                                {
                                    file_size[i++] = *file_ptr++;// ÎÄ¼ş´óĞ¡ÇøÓò µ½ file_ptr = ' 'µÄµØ·½
                                }
                                file_size[i++] = '\0';//Ìí¼Ó½áÊø·û
                                Str2Int(file_size, &size);// ×Ö·û´®´óĞ¡×ª»»³É   Êı×Ö´óĞ¡ size

                                //²âÊÔÊı¾İ°üÊÇ·ñ¹ı´ó
                                if (size > (FLASH_SIZE - ApplicationAddress_offset - 1)) 
																	// ÓĞµãÎÊÌâFLASH_SIZE = (0x20000)  /* 128 KBytes */ ÎªÈ«²¿´óĞ¡ Ó¦¸ÃÒª¼õÈ¥ IAP³ÌĞò´óĞ¡ ApplicationAddress_offset
                                {
                                    //½áÊø
                                    Send_Byte(CA);
                                    Send_Byte(CA);
                                    return -1; //ÎÄ¼ş³¬¹ıFLASH ÓÃ»§ÇøÓò´óĞ¡
                                }

                                //¼ÆËãĞèÒª²Á³ıFlashµÄÒ³
                                NbrOfPage = FLASH_PagesMask(size);
      /////////// Ö±½Ó ²Á³ıFlashÊÇ²»ÊÇÓĞÎÊÌâ  Ó¦¸ÃÏÈÅĞ¶ÏÒ»ÏÂ ½ÓÊÜµÄÎÄ¼şÊÇ·ñÓĞĞ§ ÔÙ²Á³ı£¿£¿£¿£¿
								//ÅĞ¶Ï crcĞ£ÑéÂëÊÇ·ñÕıÈ·
                 ////////////////////////²Á³ıFlash//////////////////////////
                                for (EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
                                {
                                    FLASHStatus = FLASH_ErasePage(FlashDestination + (PageSize * EraseCounter));
                                }
                                Send_Byte(ACK);//»ØÓ¦
                                Send_Byte(CRC16);//ÔÙ·¢ËÍ 'C'
                            }
                            //ÎÄ¼şÃûÊı¾İ°ü¿Õ£¬½áÊø´«Êä
                            else  // Êı¾İÄÚÈİÎª 0 ÒÑ¾­·¢ËÍÍê³É
                            {
                                Send_Byte(ACK);
                                file_done = 1;
                                session_done = 1;
                                break;
                            }
                        }
												
//////////////////////½ÓÊÕÊµ¼ÊÎÄ¼şÊı¾İ °ü
                        else
                        {
                            memcpy(buf_ptr, packet_data + PACKET_HEADER, packet_length);//Ö±½Ó¿½±´ Êı¾İ°üµÄ ÓĞĞ§ÇøÓò
                            RamSource = (uint32_t)buf;
                            for (j = 0; (j < packet_length) && (FlashDestination <  ApplicationAddress + size); j += 4)
                            {
                                //°Ñ½ÓÊÕµ½µÄÊı¾İ±àĞ´µ½FlashÖĞ
                                FLASH_ProgramWord(FlashDestination, *(uint32_t*)RamSource);

                                if (*(uint32_t*)FlashDestination != *(uint32_t*)RamSource)
                                {
                                    //½áÊø
                                    Send_Byte(CA); // ·¢ËÍ
                                    Send_Byte(CA);
                                    return -2;
                                }
                                FlashDestination += 4;// flashµØÖ·°´4×Ö½ÚÔö³¤ ÇÒÎªÅ¼Êı
                                RamSource += 4;       // ĞèÒªĞ´ÈëµÄ¶ş½øÖÆÎÄ¼şµØÖ·Ò²Í¬Ôö³¤
                            }
                            Send_Byte(ACK);//»ØÓ¦
                        }
                        packets_received ++;//Êı¾İ°ü±àºÅ+1
                        session_begin = 1;
                    }
                }
                break;
            case 1://½ÓÊÕÊı¾İÖÕÖ¹
                Send_Byte(CA);
                Send_Byte(CA);
                return -3;
            default:
                if (session_begin > 0)
                {
                    errors ++;
                }
                if (errors > MAX_ERRORS)
                {
                    Send_Byte(CA);
                    Send_Byte(CA);
                    return 0;
                }
                Send_Byte(CRC16);
                break;
            }
            if (file_done != 0)
            {
                break;
            }
        }
        if (session_done != 0)
        {
            break;
        }
    }
    return (int32_t)size;
}

/*******************************************************************************
  * @º¯ÊıÃû³Æ	Ymodem_CheckResponse
  * @º¯ÊıËµÃ÷   Í¨¹ı ymodemĞ­Òé¼ì²âÏìÓ¦
  * @ÊäÈë²ÎÊı   c
  * @Êä³ö²ÎÊı   ÎŞ
  * @·µ»Ø²ÎÊı   0
*******************************************************************************/
int32_t Ymodem_CheckResponse(uint8_t c)
{
    return 0;
}

/*******************************************************************************
  * @º¯ÊıÃû³Æ	Ymodem_PrepareIntialPacket
  * @º¯ÊıËµÃ÷   ×¼±¸µÚÒ»¸öÊı¾İ°ü     µÚÒ»¸öÊı¾İ°ü µÄÊı¾İÇøÓòÎª   ÎÄ¼şÃû ºÍÎÄ¼ş´óĞ¡ ²»°üº¬Êı¾İ
  * @ÊäÈë²ÎÊı   data£ºÊı¾İ°üÆğÊ¼µØÖ·
                fileName £ºÎÄ¼şÃû
                length £ºÎÄ¼ş´óĞ¡
  * @Êä³ö²ÎÊı   ÎŞ
  * @·µ»Ø²ÎÊı   ÎŞ
*******************************************************************************/
void Ymodem_PrepareIntialPacket(uint8_t *data, const uint8_t* fileName, uint32_t *length)
{
    uint16_t i, j;
    uint8_t file_ptr[10];

    //ÖÆ×÷Í·3¸öÊı¾İ°ü
    data[0] = SOH;//±êÊ¶Êı¾İ°ü³¤¶È128
    data[1] = 0x00;
    data[2] = 0xff;
    //ÎÄ¼şÃûÊı¾İ°üÓĞĞ§Êı¾İ
	  //´ÓµÚÈı×Ö½Ú¿ªÊ¼´æ´¢ÎÄ¼şÃû
    for (i = 0; (fileName[i] != '\0') && (i < FILE_NAME_LENGTH); i++)
    {
        data[i + PACKET_HEADER] = fileName[i];//ÎÄ¼şÃû ×î³¤Îª 256×Ö½Ú
    }
		//´ËÊ±iÎªÎÄ¼şÃû´æ´¢ºóµÄÆ«ÒÆÁ¿
    //½Ó×ÅÎÄ¼şÃû´æ´¢ºóµÄµØÖ·¿ªÊ¼ ´æ´¢ ÎÄ¼ş´óĞ¡
    data[i + PACKET_HEADER] = 0x00;//  ÔÚÎÄ¼şÃûºó×îºó ²¹0  ÓÃÓÚ½ÓÊÕÊ±Çø·Ö

    Int2Str (file_ptr, *length);//ÎÄ¼ş´óĞ¡ ×ª³É ×Ö·û´®´æ·Åµ½ file_ptrÖĞ
    for (j =0, i = i + PACKET_HEADER + 1; file_ptr[j] != '\0' ; )
    {
        data[i++] = file_ptr[j++];//´æ·ÅÎÄ¼ş´óĞ¡
    }
		// ºóÃæÊÇ²»ÊÇÈ±ÉÙ  ' ' ¿Õ¸ñ·ûºÅ£¿£¿
///////////////////////////////////////////////////////////////
		//°´½ÓÊÕÊ±µÄ¸ñÊ½ ¿´Ó¦¸ÃÈ±ÉÙÒ»¸ö ¿Õ¸ñ·û ÓÃÒÔÇø·Ö ºóÃæÎªÁãµÄÇøÓò
		data[i + PACKET_HEADER] = ' ';//  ÔÚÎÄ¼şÃûºó×îºó ²¹0  ÓÃÓÚ½ÓÊÕÊ±Çø·
    for (j = i+1; j < PACKET_SIZE + PACKET_HEADER; j++)
    {
        data[j] = 0;//µÚÒ»¸öÊı¾İ°ü Êı¾İÎ» ÔÚ ´æ·Å ÎÄ¼şÃûºÍÎÄ¼ş´óĞ¡ºóÈç¹û²»Âú£¬¾Í²¹ 0
    }
////////////////////////////////////////////////////////////
/*		
    for (j = i; j < PACKET_SIZE + PACKET_HEADER; j++)
    {
        data[j] = 0;//µÚÒ»¸öÊı¾İ°ü Êı¾İÎ» ÔÚ ´æ·Å ÎÄ¼şÃûºÍÎÄ¼ş´óĞ¡ºóÈç¹û²»Âú£¬¾Í²¹ 0
    }
*/		 
}

/*******************************************************************************
  * @º¯ÊıÃû³Æ	Ymodem_PreparePacket
  * @º¯ÊıËµÃ÷   ×¼±¸Êı¾İ°ü
  * @ÊäÈë²ÎÊı   SourceBuf£ºÊı¾İÔ´»º³å
                data£ºÊı¾İ°ü
                pktNo £ºÊı¾İ°ü±àºÅ
                sizeBlk £ºÊı¾İ°ü³¤¶È
  * @Êä³ö²ÎÊı   ÎŞ
  * @·µ»Ø²ÎÊı   ÎŞ
*******************************************************************************/
void Ymodem_PreparePacket(uint8_t *SourceBuf, uint8_t *data, uint8_t pktNo, uint32_t sizeBlk)
{
    uint16_t i, size, packetSize;
    uint8_t* file_ptr;

    //ÖÆ×÷Í·3¸ö×Ö½ÚµÄ Êı¾İ°üÍ·
    packetSize = sizeBlk >= PACKET_1K_SIZE ? PACKET_1K_SIZE : PACKET_SIZE;
    size = sizeBlk < packetSize ? sizeBlk :packetSize;
	// µÚÒ»¸ö×Ö½Ú ÎªÊı¾İÇøÓò´óĞ¡ ±íÊ¾ 128/1024
    if (packetSize == PACKET_1K_SIZE)
    {
        data[0] = STX;
    }
    else
    {
        data[0] = SOH;
    }
// µÚ¶ş¸ö×Ö½Ú
    data[1] = pktNo;   //Êı¾İ°ü ±àÂë ¼ÇÂ¼·¢ËÍ´ÎÊı
// µÚÈı¸ö×Ö½Ú
    data[2] = (~pktNo);//±àÂëµÄ·´Âë
    file_ptr = SourceBuf;//ÎÄ¼şÊı¾İ

//////Êı¾İ°üÄÚµÄÓĞĞ§Êı¾İ ´ÓµÚÈı¸ö×Ö½Ú¿ªÊ¼ ´æÈëÊı¾İ
    for (i = PACKET_HEADER; i < size + PACKET_HEADER; i++)
    {
        data[i] = *file_ptr++;//Ğ´ÈëÎÄ¼şÊı¾İ
    }
		// Ó¦¶Ô×îºóÒ»¸öÊı¾İ°ü ¿ÉÄÜÊı¾İ´óĞ¡Ğ¡Óë Ò»°üµÄÊı¾İ´óĞ¡
    if ( size  <= packetSize) //Êı¾İ²»¹»µÄ»°   ²¹»»ĞĞ
    {
        for (i = size + PACKET_HEADER; i < packetSize + PACKET_HEADER; i++)
        {
            data[i] = 0x1A; //½áÊø   
        }
    }
}

/*******************************************************************************
  * @º¯ÊıÃû³Æ	UpdateCRC16
  * @º¯ÊıËµÃ÷   ¸üĞÂÊäÈëÊı¾İµÄ£Ã£Ò£ÃĞ£Ñé
  * @ÊäÈë²ÎÊı   crcIn
                byte
  * @Êä³ö²ÎÊı   ÎŞ
  * @·µ»Ø²ÎÊı   £Ã£Ò£ÃĞ£ÑéÖµ
*******************************************************************************/
uint16_t UpdateCRC16(uint16_t crcIn, uint8_t byte)
{
    uint32_t crc = crcIn;
    uint32_t in = byte|0x100;
    do
    {
        crc <<= 1;
        in <<= 1;
        if (in&0x100)
            ++crc;
        if (crc&0x10000)
            crc ^= 0x1021;
    }
    while (!(in&0x10000));
    return crc&0xffffu;
}

/*******************************************************************************
  * @º¯ÊıÃû³Æ	  Cal_CRC16
  * @º¯ÊıËµÃ÷  ¼ÆËãÊäÈëÊı¾İµÄCRCĞ£ÑéÂë
  * @ÊäÈë²ÎÊı   data £ºÊı¾İ
                size £º³¤¶È
  * @Êä³ö²ÎÊı   ÎŞ
  * @·µ»Ø²ÎÊı   CRC Ğ£ÑéÖµ
*******************************************************************************/
uint16_t Cal_CRC16(const uint8_t* data, uint32_t size)
{
    uint32_t crc = 0;
    const uint8_t* dataEnd = data+size;
    while (data<dataEnd)
        crc = UpdateCRC16(crc,*data++);

    crc = UpdateCRC16(crc,0);
    crc = UpdateCRC16(crc,0);
    return crc&0xffffu;
}


/*******************************************************************************
  * @º¯ÊıÃû³Æ	CalChecksum
  * @º¯ÊıËµÃ÷   ¼ÆËãYModemÊı¾İ°üµÄ×Ü´óĞ¡
  * @ÊäÈë²ÎÊı   data £ºÊı¾İ
                size £º³¤¶È
  * @Êä³ö²ÎÊı   ÎŞ
  * @·µ»Ø²ÎÊı   Êı¾İ°üµÄ×Ü´óĞ¡
*******************************************************************************/
uint8_t CalChecksum(const uint8_t* data, uint32_t size)
{
    uint32_t sum = 0;
    const uint8_t* dataEnd = data+size;
    while (data < dataEnd )
        sum += *data++;
    return sum&0xffu;
}

/*******************************************************************************
  * @º¯ÊıÃû³Æ	Ymodem_SendPacket
  * @º¯ÊıËµÃ÷   Í¨¹ıymodemĞ­Òé´«ÊäÒ»¸öÊı¾İ°ü
#  ´®¿Ú´«Êä Êı¾İ
  * @ÊäÈë²ÎÊı   data £ºÊı¾İµØÖ·Ö¸Õë
                length£º³¤¶È
  * @Êä³ö²ÎÊı   ÎŞ
  * @·µ»Ø²ÎÊı   ÎŞ
*******************************************************************************/
void Ymodem_SendPacket(uint8_t *data, uint16_t length)
{
    uint16_t i;
    i = 0;
    while (i < length)
    {
        Send_Byte(data[i]);
        i++;
    }
}

/*******************************************************************************
  * @º¯ÊıÃû³Æ	Ymodem_Transmit
  * @º¯ÊıËµÃ÷   Í¨¹ıymodemĞ­Òé´«ÊäÒ»¸öÎÄ¼ş
  * @ÊäÈë²ÎÊı   buf £ºÊı¾İµØÖ·Ö¸Õë
                sendFileName £ºÎÄ¼şÃû
                sizeFile£ºÎÄ¼ş³¤¶È
  * @Êä³ö²ÎÊı   ÎŞ
  * @·µ»Ø²ÎÊı   ÊÇ·ñ³É¹¦
                0£º³É¹¦
*******************************************************************************/
uint8_t Ymodem_Transmit (uint8_t *buf, const uint8_t* sendFileName, uint32_t sizeFile)
{

    uint8_t packet_data[PACKET_1K_SIZE + PACKET_OVERHEAD];//Õı³£Êı¾İ°ü³¤¶È
    uint8_t FileName[FILE_NAME_LENGTH];//ÎÄ¼şÃû
    uint8_t *buf_ptr, tempCheckSum ;
    uint16_t tempCRC, blkNumber;
    uint8_t receivedC[2], CRC16_F = 0, i;
    uint32_t errors, ackReceived, size = 0, pktSize;

    errors = 0;
    ackReceived = 0;
    for (i = 0; i < (FILE_NAME_LENGTH - 1); i++)
    {
        FileName[i] = sendFileName[i];//±£´æÎÄ¼şÃû
    }
    CRC16_F = 1;// ĞèÒªĞ£ÑéÂë

    //×¼±¸µÚÒ»¸öÊı¾İ°ü           Êı¾İ°üÆğÊ¼µØÖ·   ÎÄ¼şÃû   ÎÄ¼ş´óĞ¡
    Ymodem_PrepareIntialPacket(&packet_data[0], FileName, &sizeFile);

    do
    {
///////·¢ËÍµÚÒ»¸öÊı¾İ°ü ÎÄ¼şÃû   ÎÄ¼ş´óĞ¡/////////////////////////////////////////////////////////////////////
        Ymodem_SendPacket(packet_data, PACKET_SIZE + PACKET_HEADER);//128 ´óĞ¡+3
        //·¢ËÍÁ½¸ö×Ö½ÚµÄ CRCĞ£ÑéÂë  ¸ß°ËÎ»ÔÚÇ°  µÍ°ËÎ»ÔÚºó
			  // Êı¾İ°üºóÔÙ·¢ËÍ  Á½¸ö×Ö½ÚµÄ  CRCĞ£ÑéÂë  »òÕß ½ö½öÊÇ Êı¾İÇøÓòµÄºÍµÄºó°ËÎ»
        if (CRC16_F)
        {
            tempCRC = Cal_CRC16(&packet_data[3], PACKET_SIZE);//´ÓÊı¾İÇøÓò¿ªÊ¼²ú³ö Á½¸ö×Ö½ÚµÄ CRCĞ£ÑéÂë
            Send_Byte(tempCRC >> 8);//ÏÈ·¢ËÍ¸ß°ËÎ»
            Send_Byte(tempCRC & 0xFF);//ÔÙ·¢ËÍµÍ°ËÎ»
        }
        else //·¢ËÍ Êı¾İÇøÓòµÄºÍµÄºó°ËÎ»
        {
            tempCheckSum = CalChecksum (&packet_data[3], PACKET_SIZE);
            Send_Byte(tempCheckSum);
        }

        //µÈ´ıÊı¾İ½ÓÊÕ¶Ë ÏìÓ¦  ACK
        if (Receive_Byte(&receivedC[0], 10000) == 0)
        {
            if (receivedC[0] == ACK)
            {
                //µÚÒ»¸ö Êı¾İ°üÕıÈ·´«Êä
                ackReceived = 1;
            }
        }
        else
        {
            errors++;
        }
    } while (!ackReceived && (errors < 0x0A)); //µÚÒ»¸öÊı¾İ°ü´«ÊäÕıÈ· ºó ÒÔ¼° ´íÎó´ÎÊı¹ı¶àºó ¾Í²»·¢ËÍµÚÒ»¸öÊı¾İ°ü

		// Èç¹ûÊÇ µÚÒ»¸öÊı¾İ°ü·¢ËÍ´íÎó´ÎÊı¹ı¶à  ¾ÍÈ¡Ïû·¢ËÍÎÄ¼ş  ²¢·µ»Ø´íÎó´ÎÊı
    if (errors >=  0x0A)
    {
        return errors;
    }
		
///////µÚÒ»¸öÊı¾İ°ü·¢ËÍÍê³ÉÖ®ºó  ¿ªÊ¼´«ÊäºóÃæµÄ ÎÄ¼şÊı¾İ°ü
    buf_ptr = buf; //ÎÄ¼ş´æ·ÅµÄ Ê×µØÖ·
    size = sizeFile;//ÎÄ¼ş´óĞ¡
    blkNumber = 0x01;//Êı¾İ°üµÄ±àºÅ
		
////////////////////////////////////////////////////////////////////////////////
////·¢ËÍºóÃæµÄÊı¾İ 1024×Ö½ÚµÄÊı¾İ°ü·¢ËÍ///////////////////
    while (size) 
    {
        //Ã¿´Î·¢ËÍÇ° ×öµÄ×¼±¸¹¤×÷
	//×¼±¸ÏÂÒ»¸öÊı¾İ°ü   ÊÕ¾İÊ×µØÖ·    Éú³ÉµÄÒ»°üÊı¾İ   Êı¾İ²»¹»µÄ»° Êı¾İÇøÓò²¹³ä »»ĞĞ·û
        Ymodem_PreparePacket(buf_ptr, &packet_data[0], blkNumber, size);
        ackReceived = 0;//Êı¾İ»ØÓ¦±êÖ¾ ÇåÁã
        receivedC[0]= 0;//»ØÓ¦ÄÚÈİÇåÀí
        errors = 0;     //´íÎó´ÎÊıÇåÁã
        do
        {
        //·¢ËÍÏÂÒ»¸öÊı¾İ°ü
		//È·¶¨Êı¾İ°ü´óĞ¡
            if (size >= PACKET_1K_SIZE)//Êı¾İ³¬¹ı 1024´óĞ¡ ¾Í°´ 1024µÄÊı¾İ°ü·¢ËÍ
            {
                pktSize = PACKET_1K_SIZE;

            }
            else
            {
                pktSize = PACKET_SIZE;//·ñÕß°´128´óĞ¡·¢ËÍ
            }
		//Í¨¹ıÊı¾İ½Ó¿Ú(´®¿ÚµÈ)·¢ËÍ×¼±¸ºÃµÄÒ»°üÊı¾İ
            Ymodem_SendPacket(packet_data, pktSize + PACKET_HEADER);
            //·¢ËÍCRCĞ£Ñé
            if (CRC16_F)
            {
                tempCRC = Cal_CRC16(&packet_data[3], pktSize);
                Send_Byte(tempCRC >> 8);   //¸ß°ËÎ»
                Send_Byte(tempCRC & 0xFF); //µÍ°ËÎ»
            }
            else
            {
                tempCheckSum = CalChecksum (&packet_data[3], pktSize);
                Send_Byte(tempCheckSum);//Êı¾İºÍµÍ°ËÎ»
            }

            //µÈ´ı½ÓÊÕ¶Ë»ØÓ¦
            if ((Receive_Byte(&receivedC[0], 100000) == 0)  && (receivedC[0] == ACK))
            {
                ackReceived = 1; //ÒÑ»ØÓ¦
                if (size > pktSize) //Ê£ÓàÎ´·¢ËÍÊı¾İ ³¬¹ıÒ»¸öÊı¾İ°ü´óĞ¡Âğ
                {
                    buf_ptr += pktSize;// Êı¾İµØÖ·Æ«ÒÆ
                    size -= pktSize;   // Î´·¢ËÍÊı¾İ´óĞ¡ÏàÓ¦¼õĞ¡
                    if (blkNumber == (FLASH_IMAGE_SIZE/1024))//Èç¹û³¬¹ı¿ÉÓÃflashµØÖ·´óĞ¡·µ»Ø´íÎó
                    {
                        return 0xFF; //´íÎó
                    }
                    else
                    {
                        blkNumber++;//Êı¾İ°ü ±àºÅ+1
                    }
                }
                else//Êı¾İÒÑ¾­·¢ËÍÍêÁË  ÒòÎªÉú³ÉÊı¾İ°üÔÚÇ°  Êı¾İ°ü´óĞ¡¼õĞ¡ÔÚºó
                {
                    buf_ptr += pktSize;
                    size = 0; // Êı¾İ´óĞ¡ÖÃ0 ½áÊøÊı¾İµÄ·¢ËÍ
                }
            }
            else
            {
                errors++;//Î´»ØÓ¦  ´íÎó´ÎÊı+1
            }
        } while (!ackReceived && (errors < 0x0A));
				
        //Èç¹ûÃ»ÏìÓ¦10´Î¾Í·µ»Ø´íÎó
        if (errors >=  0x0A)
        {
            return errors;
        }

    }
//////////////////////////////////////////		
    ackReceived = 0;//»ØÓ¦±êÖ¾ÇåÁã
    receivedC[0] = 0x00;//»ØÓ¦ÄÚÈİÇåÀí
    errors = 0;         //Î´»ØÓ¦´íÎó´ÎÊıÇåÁã
    do
    {
        Send_Byte(EOT);//·¢ËÍ½áÊø·¢ËÍ±êÖ¾
        //·¢ËÍ (EOT);
        //µÈ´ı»ØÓ¦
        if ((Receive_Byte(&receivedC[0], 10000) == 0)  && receivedC[0] == ACK)
        {
            ackReceived = 1;//ÒÑ»ØÓ¦±êÖ¾
        }
        else
        {
            errors++;//Î´»ØÓ¦´íÎó´ÎÊı+1
        }
    } while (!ackReceived && (errors < 0x0A));

    if (errors >=  0x0A)
    {
        return errors;
    }
		
////////////////////////////////////////////////////////////////////////
////  ·¢ËÍ ½áÊø·¢ËÍÖ®ºó ĞèÒª·¢ËÍÒ»¸öÈ«Îª0µÄÊı¾İ°ü
    ackReceived = 0;
    receivedC[0] = 0x00;
    errors = 0;
////Êı¾İ°üÇ°Èı¸ö×Ö½Ú
    packet_data[0] = SOH;//128×Ö½Ú´óĞ¡
    packet_data[1] = 0;
    packet_data [2] = 0xFF;
//×¼±¸Êı¾İÇøÓò 
    for (i = PACKET_HEADER; i < (PACKET_SIZE + PACKET_HEADER); i++)
    {
        packet_data [i] = 0x00;//È«Îª 0
    }

    do
    {
      	//Í¨¹ıÊı¾İ½Ó¿Ú(´®¿ÚµÈ)·¢ËÍ×¼±¸ºÃµÄÒ»°üÊı¾İ
        Ymodem_SendPacket(packet_data, PACKET_SIZE + PACKET_HEADER);
			
        //·¢ËÍCRCĞ£Ñé
        tempCRC = Cal_CRC16(&packet_data[3], PACKET_SIZE);
        Send_Byte(tempCRC >> 8);
        Send_Byte(tempCRC & 0xFF);

        //µÈ´ıÏìÓ¦
        if (Receive_Byte(&receivedC[0], 10000) == 0)
        {
            if (receivedC[0] == ACK)
            {
                //°ü´«ÊäÕıÈ·
                ackReceived = 1;
            }
        }
        else
        {
            errors++;
        }

    } while (!ackReceived && (errors < 0x0A));
    //Èç¹ûÃ»ÏìÓ¦10´Î¾Í·µ»Ø´íÎó
    if (errors >=  0x0A)
    {
        return errors;
    }

		ackReceived = 0;
    receivedC[0] = 0x00;
    errors = 0;
		//×îºóÔÙÒ»´Î·¢ËÍ½áÊø·¢ËÍ±êÖ¾
    do
    {
        Send_Byte(EOT);
        //·¢ËÍ (EOT);
        //µÈ´ı»ØÓ¦
        if ((Receive_Byte(&receivedC[0], 10000) == 0)  && receivedC[0] == ACK)
        {
            ackReceived = 1;
        }
        else
        {
            errors++;
        }
    } while (!ackReceived && (errors < 0x0A));

    if (errors >=  0x0A)
    {
        return errors;
    }
    return 0;//ÎÄ¼ş´«Êä³É¹¦
}

/*******************************ÎÄ¼ş½áÊø***************************************/
