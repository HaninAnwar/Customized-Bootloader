


#ifndef  UART_CONFIG_H
#define  UART_CONFIG_H


/************************************************************************************/
/******************************** Baud Rate *****************************************/
/************************************************************************************/
#define F_COLCK                   16000000UL
#define USART1_BAUD_RATE         (uint32_t)115200
/************************************************************************************/
/***************************** USART Numbers Configuration **************************/
/************************************************************************************/
#define  USART_NUMBERS          6
static   USART_REG* USART_Index[USART_NUMBERS]={USART1,USART2,USART3,UART4,UART5,USART6};
/************************************************************************************/
/************************************************************************************/
/************************************************************************************/








#endif
