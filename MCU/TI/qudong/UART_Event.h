/*
 * UART_Event.h
 *
*/

#ifndef UART_EVENT_H_
#define UART_EVENT_H_

extern void UART_OnTx(void);
extern void UART_OnRx(void);
extern void UART_SendString(const  unsigned char *Ptr); //给上位机发送字符串


#endif /* UART_EVENT_H_ */
