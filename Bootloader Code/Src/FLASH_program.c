#include <stdint.h>
#include "STM32F446xx.h"
#include "ERROR_STATE.h"


#include "FLASH_interface.h"
#include "FLASH_private.h"

void FLASH_voidLock(void)
{
	FLASH_INTERFACE->CR |= FLASH_LOCK_VAL << FLASH_LOCK_SHIFT;
}
void FLASH_voidUnlockFlash(void)
{
	FLASH_INTERFACE->KEYR = FLASH_UNLOCK_FLASH_KEY1;
	FLASH_INTERFACE->KEYR = FLASH_UNLOCK_FLASH_KEY2;
}

void Flash_voidUnlockOTP(void)
{
	FLASH_INTERFACE->OPTKEYR = FLASH_UNLOCK_OTP_KEY1;
	FLASH_INTERFACE->OPTKEYR = FLASH_UNLOCK_OTP_KEY2;

}
void Flash_voidLockOTP(void)
{
	FLASH_INTERFACE->OPTCR |= 1;
}
uint8_t FLASH_u8IsBusy(void)
{
	uint8_t Local_u8FlashStatus;
	uint64_t Local_u64Counter = 0;
	while((((FLASH_INTERFACE->SR & (1<<FLASH_BUSY_SHIFT)) >> FLASH_BUSY_SHIFT) == FLASH_IS_BUSY) && (Local_u64Counter < FLASH_TIMEOUT))
	{
		Local_u64Counter++;
	}

	if(Local_u64Counter == FLASH_TIMEOUT)
	{
		Local_u8FlashStatus = FLASH_IS_BUSY;
	}
	else
	{
		Local_u8FlashStatus = FLASH_IS_READY;
	}

	return Local_u8FlashStatus;
}
uint8_t FLASH_u8Erase(Flash_Erase_TypeDef *Copy_FlashErase)
{
	uint8_t Local_u8Errorstate = OK,
			Local_u8FlashStatus;

	/* Wait until the flash is ready */
	Local_u8FlashStatus = FLASH_u8IsBusy();

	if(Local_u8Errorstate == FLASH_IS_READY)
	{

		/*Set erase type*/
		if(Copy_FlashErase->EraseType == FLASH_SECTOR_ERASE)
		{
			/* Set the parallelism*/
			FLASH_INTERFACE->CR |= Copy_FlashErase->Parallelism << FLASH_PARALLEL_SHIFT;

			for(uint8_t Local_u8Sector = Copy_FlashErase->StartSector; Local_u8Sector < Copy_FlashErase->EndSector; Local_u8Sector++)
			{
				/* Wait until the flash is ready */
				Local_u8FlashStatus = FLASH_u8IsBusy();

				if(Local_u8FlashStatus == FLASH_IS_READY)
				{
					/* Set the desired sector*/
					FLASH_INTERFACE->CR |= Local_u8Sector << FLASH_ERASE_SNB_SHIFT;

					/* Sector erase*/
					FLASH_INTERFACE->CR |= 1 << FLASH_SECTOR_ERASE_EN_SHIFT;

					/* Start erasing*/
					FLASH_INTERFACE->CR |= 1 << FLASH_ERASE_STRT_SHIFT;
				}
				else
				{
					Local_u8Errorstate = NOK;
				}
			}

		}
		else if(Copy_FlashErase->EraseType == FLASH_MASS_ERASE)
		{
			/* Mass erase*/
			FLASH_INTERFACE->CR |= 1 << FLASH_MASS_ERASE_EN_SHIFT;

			/* Start erasing*/
			FLASH_INTERFACE->CR |= 1 << FLASH_ERASE_STRT_SHIFT;
		}
		else
		{
			Local_u8Errorstate = NOK;
		}
		/*Clear SNB and PSIZE*/
		FLASH_INTERFACE->CR = 0;

	}

	else
	{
		Local_u8Errorstate = NOK;
	}

	return Local_u8Errorstate;
}
uint8_t FLASH_u8Program(Flash_Write_TypeDef *Copy_FlashProgram)
{
	uint8_t Local_u8Errorstate = OK,
			Local_u8FlashStatus;

	/* Wait until the flash is ready */
	Local_u8FlashStatus = FLASH_u8IsBusy();

	if(Local_u8Errorstate == FLASH_IS_READY)
	{
		/* Set the parallelism*/
		FLASH_INTERFACE->CR |= Copy_FlashProgram->Parallelism << FLASH_PARALLEL_SHIFT;

		/* Enable program bit*/
		FLASH_INTERFACE->CR |= 1;

		/* Check the data size according to parallelism*/
		switch(Copy_FlashProgram->Parallelism)
		{
		/*Start writing*/
		case FLASH_PAR_BYTE		:*(volatile uint8_t *)Copy_FlashProgram->BaseAddress = (uint8_t )Copy_FlashProgram->Data;	break;

		case FLASH_PAR_HWORD 	:*(volatile uint16_t *)Copy_FlashProgram->BaseAddress = (uint16_t )Copy_FlashProgram->Data;	break;

		case FLASH_PAR_WORD 	:*(volatile uint32_t *)Copy_FlashProgram->BaseAddress = (uint32_t )Copy_FlashProgram->Data;	break;

		case FLASH_PAR_DWORD 	:
			*(uint32_t *)Copy_FlashProgram->BaseAddress = (uint32_t )Copy_FlashProgram->Data;
			*(uint32_t *)(Copy_FlashProgram->BaseAddress+4) = (uint32_t )(Copy_FlashProgram->Data >> 32);
			break;

		default : Local_u8Errorstate = NOK;
		}

		/* Wait until the flash is ready */
		Local_u8FlashStatus = FLASH_u8IsBusy();
		if(Local_u8FlashStatus == FLASH_IS_BUSY)
		{
			Local_u8Errorstate = NOK;
		}

		/*Clear SNB and PSIZE*/
		FLASH_INTERFACE->CR = 0;
	}
	else
	{
		Local_u8Errorstate = NOK;
	}
	return Local_u8Errorstate;
}

uint8_t Flash_u8EnRWProtection(uint8_t Copy_u8ProtectSectors, uint8_t Copy_u8ProtectMode)
{
	uint8_t Local_u8ErrorStatus = OK;

	uint8_t Local_u8FlashStatus;


	/* Unlock Option byte*/
	Flash_voidUnlockOTP();


	/* Wait on the busy flag in flash status register*/
	Local_u8FlashStatus = FLASH_u8IsBusy();

	if(Local_u8FlashStatus == FLASH_IS_READY)
	{
		if(Copy_u8ProtectMode == WRITE_PROTECT)
		{
			/* Write protection is required*/

			/* Clear SPRMOD bit*/
			FLASH_INTERFACE->OPTCR &= ~( 1 << FLASH_SPRMOD_SHIFT) ;

			/*  Reversing Copy_u8ProtectSectors bits : 0 -> active, 1 -> not acgive */
			Copy_u8ProtectSectors = ~Copy_u8ProtectSectors;

		}
		else if (Copy_u8ProtectMode == READ_PROTECT)
		{
			/* Read protection is required */


			/* Change RDP to level 1*/
			FLASH_INTERFACE->OPTCR |= 0xFF << 8;

			/* Set SPRMOD bit*/
			FLASH_INTERFACE->OPTCR |= ( 1 << FLASH_SPRMOD_SHIFT) ;

		}

		/* Clear nWRP bits */
		FLASH_INTERFACE->OPTCR &=~( FLASH_nWRP_MASK << FLASH_nWRP_SHIFT);

		/* Put the desired protected sectors in nWRP bits*/
		FLASH_INTERFACE->OPTCR |= Copy_u8ProtectSectors << FLASH_nWRP_SHIFT;


		/* Set OPTSTRT bit */
		FLASH_INTERFACE->OPTCR |= 1 << FLASH_OTPSTRT_SHIFT;

		/* Wait till the flash finishes (busy flag)*/
		Local_u8FlashStatus = FLASH_u8IsBusy();

		/* Lock the Option byte*/
		Flash_voidLockOTP();
	}
	else
	{
		Local_u8ErrorStatus = NOK;
	}
	return Local_u8ErrorStatus;

}
uint8_t Flash_u8DisAllRWProtection(void)
{
	uint8_t Local_u8ErrorState = OK;
	uint8_t Local_u8FlashStatus;

	/* Unlock OPT */
	Flash_voidUnlockOTP();

	/* Check flash status */
	Local_u8FlashStatus = FLASH_u8IsBusy();

	if(Local_u8FlashStatus == FLASH_IS_READY)
	{
		if(((FLASH_INTERFACE->OPTCR &(1 << FLASH_SPRMOD_SHIFT)) >> FLASH_SPRMOD_SHIFT) == 0)
		{

			/* Clear SPRMOD bit */
			FLASH_INTERFACE->OPTCR &= ~(1 << FLASH_SPRMOD_SHIFT);

			/* Disable write protection for all sectors*/
			FLASH_INTERFACE->OPTCR |= (FLASH_nWRP_MASK << FLASH_nWRP_SHIFT);

			/* Start OPT operation */
			FLASH_INTERFACE->OPTCR |= (1 << FLASH_OTPSTRT_SHIFT);

			/* Wait until flash is finished */
			Local_u8FlashStatus = FLASH_u8IsBusy();

			/* Lock the OPT */
			Flash_voidLockOTP();

		}
//
//		else
//		{
//			uint32_t Local_u32OPTValue = FLASH_INTERFACE->OPTCR;
//
//			/* Clear SPRMOD bit */
//			Local_u32OPTValue &= ~(1 << FLASH_SPRMOD_SHIFT);
//
//			/* Disable write protection for all sectors*/
//			Local_u32OPTValue |= (FLASH_nWRP_MASK << FLASH_nWRP_SHIFT);
//
//			Local_u32OPTValue |= 0xAA << 8;
//
//			FLASH_INTERFACE->OPTCR = Local_u32OPTValue;
//
//			/* Start OPT operation */
//			FLASH_INTERFACE->OPTCR |= (1 << FLASH_OTPSTRT_SHIFT);
//
//			/* Wait until flash is finished */
//			Local_u8FlashStatus = FLASH_u8IsBusy();
//
//			/* Lock the OPT */
//			Flash_voidLockOTP();
////			uint32_t Local_u32OPTValue = FLASH_INTERFACE->OPTCR;
////
////			/* Return to Level0*/
////			Local_u32OPTValue |= 0xAA << 8;
////
////			/* Clear SPRMOD bit */
////			CLR_BIT(FLASH_INTERFACE->OPTCR,31);
////
////			/* Disable read protection for all sectors*/
//////			Local_u32OPTValue &= ~((uint32_t)(FLASH_nWRP_MASK) << FLASH_nWRP_SHIFT);
////
//////			FLASH_INTERFACE->OPTCR = Local_u32OPTValue;
////
////			/* Start OPT operation */
////			FLASH_INTERFACE->OPTCR |= (1 << FLASH_OTPSTRT_SHIFT);
////
////
////			/* Wait until flash is finished */
////			Local_u8FlashStatus = FLASH_u8IsBusy();
////
////			/* Lock the OPT */
////			Flash_voidLockOTP();
//		}


	}

	return Local_u8ErrorState;
}

uint8_t Flash_u8GetProtecStatus(void)
{
	uint8_t Local_u8ProtectStatus = (FLASH_INTERFACE->OPTCR & (FLASH_nWRP_MASK << FLASH_nWRP_SHIFT)) >> FLASH_nWRP_SHIFT;

	return Local_u8ProtectStatus;
}
