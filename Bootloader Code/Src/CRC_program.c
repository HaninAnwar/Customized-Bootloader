#include <stdint.h>
#include "STM32F446xx.h"

#include "CRC_interface.h"
#include "CRC_private.h"

uint32_t CRC_u32tCalculate(uint32_t *Copt_pu32Data,uint32_t Copy_u32Length)
{
	uint32_t Local_u32Iterator,Local_u32CRC_Val;

	/* Start calculate CRC */
	for(Local_u32Iterator = 0; Local_u32Iterator < Copy_u32Length; Local_u32Iterator++)
	{
		CRC->DR = Copt_pu32Data[Local_u32Iterator];
	}
	/* Get the accumulator CRC value*/
	Local_u32CRC_Val = CRC->DR;


	return Local_u32CRC_Val;
}

void CRC_voidReset(void)
{
	/* Reset the CRC */
	CRC->CR = 1;
}
