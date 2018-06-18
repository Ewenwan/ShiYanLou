/*
 * UART_FIFO.h
 */

#ifndef UART_FIFO_H_
#define UART_FIFO_H_

extern char Rx_FIFO_ReadChar();
extern char Rx_FIFO_WriteChar();
extern void Rx_FIFO_Clear();
extern char Tx_FIFO_ReadChar();
extern char Tx_FIFO_WriteChar();
extern void Tx_FIFO_Clear();

#endif /* UART_FIFO_H_ */
