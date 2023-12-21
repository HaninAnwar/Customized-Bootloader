/****************************************************************************/
/*
 * @file 		GPIO_program.c
 * @author		Hanin Anwar
 * @brief		the GPIO main source file, including functions' definitions
 *
 * */

#include <stdint.h>

#include "ERROR_STATE.h"

#include "../Inc/STM32F446xx.h"
#include "../Inc/GPIO_private.h"
#include "../Inc/GPIO_interface.h"

static GPIO_RegDef_T *GPIOPort[GPIO_PERIPHERAL_NUM] = {GPIOA ,GPIOB ,GPIOC ,GPIOD ,GPIOE ,GPIOF ,GPIOG ,GPIOH};

/****************************************************************************/
/*
 * @fn 			GPIO_u8PinInit
 * @brief 		the function initializes the GPIO pin according to the input parameters
 * @param[in]	PinConfig : the initialization values of the pin
 * @retval		ErrorStatus
 * */
uint8_t GPIO_u8PinInit(const GPIO_PinConfig_T *PinConfig)
{
	uint8_t Local_u8ErrorState = ERROR_STATE_OK,
			Local_u8RegNum,
			Local_u8BitNum;

	/*Check Null Pointer*/
	if(PinConfig != NULL)
	{
		/*Check Valid Inputs Configuration*/
		if((PinConfig->Port <= PORTH) && ((PinConfig->PinNum) <= PIN15)
				&& (PinConfig->Mode <= ANALOG) && (PinConfig->AltFunc <= AF15)
				&& (PinConfig->OutType <= OPEN_DRAIN) && (PinConfig->PullType <= PULL_DN)
				&&(PinConfig->Speed) <= SPEED_HIGH)
		{
			/*1. Set Pin Mode : Input ,Output ,Analog ,Alternative Function */
			(GPIOPort[PinConfig->Port]->MODER) &= ~(MODER_MASK << ((PinConfig->PinNum)*MODER_PIN_ACCESS)) ;
			(GPIOPort[PinConfig->Port]->MODER) |= ((PinConfig->Mode) << (PinConfig->PinNum)*MODER_PIN_ACCESS);

			/*2. Set Pull State : PullUp ,PullDn ,NoPull*/
			(GPIOPort[PinConfig->Port]->PUPDR) &= ~(PUPDR_MASK << (PinConfig->PinNum)*PUPDR_PIN_ACCESS);
			(GPIOPort[PinConfig->Port]->PUPDR) |= (PinConfig->PullType << (PinConfig->PinNum)*PUPDR_PIN_ACCESS);

			/*Output Pin Setting*/
			if((PinConfig->Mode == OUTPUT) || (PinConfig->Mode == ALTER_FUNC))
			{
				/*3. Set Output Type Of Pin : PushPull ,OpenDrain*/
				(GPIOPort[PinConfig->Port]->OTYPER) &= ~(OTYPER_MASK << PinConfig->PinNum);
				(GPIOPort[PinConfig->Port]->OTYPER) |= (PinConfig->OutType << PinConfig->PinNum);

				/*4. Set Speed Of Pin : Low ,Medium ,Fast ,High*/
				(GPIOPort[PinConfig->Port]->OSPEEDER) &= ~(OSPEEDER_MASK << (PinConfig->PinNum)*OSPEEDER_PIN_ACCESS);
				(GPIOPort[PinConfig->Port]->OSPEEDER) |= (PinConfig->Speed << (PinConfig->PinNum)*OSPEEDER_PIN_ACCESS);

				/*5. Set Alternative Function*/
				if(PinConfig->Mode == ALTER_FUNC)
				{
					Local_u8RegNum = (PinConfig->PinNum)/AFR_DIV_REM;
					Local_u8BitNum = ((PinConfig->PinNum)%AFR_DIV_REM)*AFR_PIN_ACCESS;

					(GPIOPort[PinConfig->Port]->AFR[Local_u8RegNum]) &= ~(AFR_MASK << Local_u8BitNum);
					(GPIOPort[PinConfig->Port]->AFR[Local_u8RegNum]) |= (PinConfig->AltFunc << Local_u8BitNum);
				}
			}
		}
		else
		{
			Local_u8ErrorState = ERROR_STATE_NOK;
		}
	}

	else
	{
		Local_u8ErrorState = ERROR_STATE_NULL_PTR;
	}

	return Local_u8ErrorState;

}

/****************************************************************************/
/*
 * @fn 			GPIO_u8SetPinValue
 * @brief 		the function set the GPIO pin according to the input value
 * @param[in]	Port : the port number, get options @Port_T enum
 * @param[in]	PinNum : the pin number ,get options @Pin_T enum
 * @param[in]	PinValue : the output value ,get options @PinVal_T
 * @retval		ErrorStatus
 * */
uint8_t GPIO_u8SetPinValue(Port_T Port , Pin_T PinNum , PinVal_T PinValue)
{
	uint8_t  Local_u8ErrorState = ERROR_STATE_OK;

	/*Check Valid Inputs Configuration*/
	if((Port <= PORTH) && (PinNum <= PIN15)
		&& (PinValue <= PIN_HIGH))
	{
		(GPIOPort[Port])->ODR &= ~(ODR_MASK << PinNum);
		(GPIOPort[Port])->ODR |= (PinValue << PinNum);
	}
	else
	{
		Local_u8ErrorState = ERROR_STATE_NOK;
	}

	return Local_u8ErrorState;
}

/****************************************************************************/
/*
 * @fn 			GPIO_u8TogglePinValue
 * @brief 		the function toggle the GPIO pin
 * @param[in]	Port : the port number, get options @Port_T enum
 * @param[in]	PinNum : the pin number ,get options @Pin_T enum
 * @retval		ErrorStatus
 * */
uint8_t GPIO_u8TogglePinValue(Port_T Port , Pin_T PinNum)
{
	uint8_t  Local_u8ErrorState = ERROR_STATE_OK;

	/*Check Valid Inputs Configuration*/
	if((Port <= PORTH) && (PinNum <= PIN15))
	{
		(GPIOPort[Port])->ODR ^= (ODR_MASK << PinNum);
	}

	else
	{
		Local_u8ErrorState = ERROR_STATE_NOK;
	}


	return Local_u8ErrorState;
}

/****************************************************************************/
/*
 * @fn 			GPIO_u8ReadPinValue
 * @brief 		the function read the GPIO input pin
 * @param[in]	Port : the port number, get options @Port_T enum
 * @param[in]	PinNum : the pin number ,get options @Pin_T enum
 * @param[out]	PinValue : the reading pin value ,get options @PinVal_T
 * @retval		ErrorStatus
 * */
uint8_t GPIO_u8ReadPinValue(Port_T Port , Pin_T PinNum , PinVal_T *PinValue)
{
	uint8_t  Local_u8ErrorState = ERROR_STATE_OK;

	/*Check Valid Inputs Configuration*/
	if((Port <= PORTH) && ((PinNum) <= PIN15))
	{
		*PinValue = (((GPIOPort[Port])->IDR >> PinNum)&IDR_ANDING);
	}
	else
	{
		Local_u8ErrorState = ERROR_STATE_NOK;
	}

	return Local_u8ErrorState;
}
