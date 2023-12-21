#ifndef  UART_PRIVATE_H
#define  UART_PRIVATE_H

/************************************************************************************/
/******************************** Baud Rate Access **********************************/
/************************************************************************************/
#define  USART_BAUD_RATE_ACCESS       4
#define  USART_MAX_FRAC               1500
#define  USART_FRACTION               0
#define  USART_MANTISSA               1
/************************************************************************************/
/******************************** Register Access ***********************************/
/************************************************************************************/
#define  USART_SR_RXNE          5
#define  USART_SR_TC            6

#define  USART_CR1_SBK          0
#define  USART_CR1_RWU          1
#define  USART_CR1_RE           2
#define  USART_CR1_TE           3
#define  USART_CR1_IDLEIE       4
#define  USART_CR1_RXNEIE       5
#define  USART_CR1_TCIE         6
#define  USART_CR1_TXEIE        7
#define  USART_CR1_PEIE         8
#define  USART_CR1_PS           9
#define  USART_CR1_PCE          10
#define  USART_CR1_WAKE         11
#define  USART_CR1_M            12
#define  USART_CR1_UE           13
#define  USART_CR1_OVER8        15

#define  USART_CR2_STOP         12

#define  USART_CR3_EIE          0
#define  USART_CR3_DMAR         6
#define  USART_CR3_DMAT         7

/************************************************************************************/
/******************************** MASK **********************************************/
/************************************************************************************/
#define  USART_SBK_MASK          0b1
#define  USART_RWU_MASK          0b1
#define  USART_RE_MASK           0b1
#define  USART_TE_MASK           0b1
#define  USART_IDLEIE_MASK       0b1
#define  USART_RXNEIE_MASK       0b1
#define  USART_TCIE_MASK         0b1
#define  USART_TXEIE_MASK        0b1
#define  USART_PEIE_MASK         0b1
#define  USART_PS_MASK           0b1
#define  USART_PCE_MASK          0b1
#define  USART_WAKE_MASK         0b1
#define  USART_M_MASK            0b1
#define  USART_OVER8_MASK        0b1
#define  USART_STOP_MASK         0b11
#define  USART_EIE_MASK          0b1
#define  USART_DMAR_MASK         0b1
#define  USART_DMAT_MASK         0b1
#define  USART_SR_MASK           0x0
#define  USART_SR_RXNE_DOWN_MASK 0b0
#define  USART_SR_RXNE_UP_MASK   0b1
#define  USART_SR_TC_MASK        0b0




/************************************************************************************/
/************************************************************************************/
/************************************************************************************/


#endif
