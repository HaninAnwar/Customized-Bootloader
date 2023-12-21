#ifndef RCC_INTERFACE_H
#define RCC_INTERFACE_H
 
typedef enum
{
    STATE_OFF   =0,
    STATE_ON    =1,
}STATE_ty;
typedef enum
{
    CLK_HSI     =0,
    CLK_HSE     =16,
    CLK_PLL     =24,
    CLK_PLLI2S  =26,
    CLK_PLLSAI  =28,
}CLKsSYS_type_ty;

typedef enum
{
    HSI     =0b0,
    HSE     =0b1,
    PLL_P   =0b10,
    PLL_R   =0b11,
}CLK_type_ty;
typedef enum
{
    AHB1_GPIOA      =0,
    AHB1_GPIOB      =1,
    AHB1_GPIOC      =2,
    AHB1_GPIOD      =3,
    AHB1_GPIOE      =4,
    AHB1_GPIOF      =5,
    AHB1_GPIOG      =6,
    AHB1_GPIOH      =7,
    AHB1_CRC        =12,
    AHB1_BKPSRAM    =18,
    AHB1_DMA1       =21,
    AHB1_DMA2       =22,
    AHB1_OTGHS      =29,
    AHB1_OTGHSULPI  =30,   
}AHB1peripheral_ty;
typedef enum
{
    APB2_TIM1      =0,
    APB2_TIM8,
    APB2_USART1    =4,
    APB2_USART6,
    APB2_ADC1      =8,
    APB2_ADC2,
    APB2_ADC3,
    APB2_SDIO,
    APB2_SPI11,
    APB2_SPI14,
    APB2_SYSCFG,
    APB2_TIM9     =16,
    APB2_TIM10,
    APB2_TIM11,
    APB2_SAI1      =22,
    APB2_SAI2, 
}APB2peripheral_ty;
typedef enum
{
    APB1_TIM2      =0,
    APB1_TIM3,
    APB1_TIM4,
    APB1_TIM5,
    APB1_TIM6,
    APB1_TIM7,
    APB1_TIM12,
    APB1_TIM13,
    APB1_TIM14,
    APB1_WWDG   =11,
    APB1_SPI2   =14,
    APB1_SPI3,
    APB1_SPDIFRX,
    APB1_USART2,
    APB1_USART3,
    APB1_UART4,
    APB1_UART5,
    APB1_I2C1,
    APB1_I2C2,
    APB1_I2C3,
    APB1_FMPI2C1,
    APB1_CAN1,
    APB1_CAN2,
    APB1_CEC,
    APB1_PWR,
    APB1_DAC, 
}APB1peripheral_ty;
uint8_t RCC_u8SetClksts(CLKsSYS_type_ty ClkType,STATE_ty State);
uint8_t RCC_SetSysClk(CLK_type_ty ClkType);
void RCC_voidAHB1EnablePerapheralClock(AHB1peripheral_ty Copy_Perapheral);
void RCC_voidAHB1DisablePerapheralClock(AHB1peripheral_ty Copy_Perapheral);
void RCC_voidAPB2EnablePerapheralClock(APB2peripheral_ty Copy_Perapheral);
void RCC_voidAPB2DisablePerapheralClock(APB2peripheral_ty Copy_Perapheral);
void RCC_voidAPB1EnablePerapheralClock(APB1peripheral_ty Copy_Perapheral);
void RCC_voidAPB1DisablePerapheralClock(APB1peripheral_ty Copy_Perapheral);
#endif