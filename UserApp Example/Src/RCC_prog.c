/***********
 * @file RCC_program.c 
 * @author Ahmed Tarek
 * @brief the RCC main source file, including functions' definitions
 * 
 */


#include <stdint.h>
#include "STM32F446xx.h"
#include "ERROR_STATE.h"

#include "RCC_prv.h"
#include "RCC_interface.h"
/******************
 * @fn RCC_SetClksts
 * @brief the function change clock state according to the input parameters
 * @param[in]  ClkType  : the clock what it is want to change his state
 * @param[in]  State : the state which the user wants the clock to be OFForON
 * @retval ErrorStatus
 */
uint8_t RCC_u8SetClksts(CLKsSYS_type_ty ClkType,STATE_ty State)
{

	uint8_t Local_ErrorState = ERROR_STATE_OK;
	uint32_t Polling=0;
	/*
	RCC->RCC_CR &=~(ONE_BIT_MASKING << ClkType);
	RCC->RCC_CR |=(State << ClkType);
	while ((((RCC->RCC_CR,CLKYYPE_FLAG) & 1)!=State)&&(Polling!=CLKYYPE_TIMEOUT))
	{
    	Polling++;
    }
	*/
	switch (State)
	{
	case STATE_ON:
		RCC->RCC_CR |=(1 << ClkType);
		while ((((RCC->RCC_CR,CLKYYPE_FLAG) & 1)!=State)&&(Polling!=CLKYYPE_TIMEOUT))
		{
        	Polling++;
    	}
		break;
	case STATE_OFF:
		RCC->RCC_CR &=~(1 << ClkType);
		/*NO need for polling*/
		break;
	default:
		Local_ErrorState= ERROR_STATE_NOK;
		break;
	}
	if(Polling==CLKYYPE_TIMEOUT)
	{
		Local_ErrorState = ERROR_STATE_TIME_OUT;
	}
	return Local_ErrorState;
}
/**********************
 * @fn RCC_SetSysClk
 * @brief the funtion change the system clock source, according to input parameter
 * @param[in] ClkType : the clock which uaer want to be a system clook
 * @retval Error status
*/
uint8_t RCC_SetSysClk(CLK_type_ty ClkType)
{
	uint8_t Local_ErrorState = ERROR_STATE_OK;
	if (ClkType <= PLL_R)
	{
		RCC->RCC_CFGR &=~(TWO_BITS_MASKING << RCC_CFGR_SW_0);
		RCC->RCC_CFGR |=(ClkType << RCC_CFGR_SW_0);
	}
	else
	{
		Local_ErrorState=ERROR_STATE_NOK;
	}
	return Local_ErrorState;
}


void RCC_voidAHB1EnablePerapheralClock(AHB1peripheral_ty Copy_Perapheral)
{
	RCC->RCC_AHB1ENR |=(ONE_BIT_MASKING<<Copy_Perapheral);
}
void RCC_voidAHB1DisablePerapheralClock(AHB1peripheral_ty Copy_Perapheral)
{
	RCC->RCC_AHB1ENR &=~(ONE_BIT_MASKING<<Copy_Perapheral);
}

void RCC_voidAPB2EnablePerapheralClock(APB2peripheral_ty Copy_Perapheral)
{
	RCC->RCC_APB2ENR |=(ONE_BIT_MASKING<<Copy_Perapheral);
}
void RCC_voidAPB2DisablePerapheralClock(APB2peripheral_ty Copy_Perapheral)
{
	RCC->RCC_APB2ENR &=~(ONE_BIT_MASKING<<Copy_Perapheral);
}

void RCC_voidAPB1EnablePerapheralClock(APB1peripheral_ty Copy_Perapheral)
{
	RCC->RCC_APB1ENR |=(ONE_BIT_MASKING<<Copy_Perapheral);
}
void RCC_voidAPB1DisablePerapheralClock(APB1peripheral_ty Copy_Perapheral)
{
	RCC->RCC_APB1ENR &=~(ONE_BIT_MASKING<<Copy_Perapheral);
}