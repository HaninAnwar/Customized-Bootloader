#include <stdint.h>
#include "ERROR_STATE.h"

#include "BL.h"
#include "BL_prv.h"

#include "USART_interface.h"
#include "CRC_interface.h"
#include "FLASH_interface.h"

void BL_voidJumpToUserApp(void)
{
	void( * AppResetHandler)(void);
	uint32_t Local_u32MSPValue,Local_u32ResetAddr;

	/* Get the MSP value of the user application*/
	Local_u32MSPValue = *((volatile uint32_t *)(FLASH_SECTOR_2_BASE_ADDR));

	/* assign this value into MSP*/
	__asm volatile("MSR MSP,%0"::"r"(Local_u32MSPValue));

	/* Get the Reset Handler address of the user application*/
	Local_u32ResetAddr = *((uint32_t *)(FLASH_SECTOR_2_BASE_ADDR+4));
	AppResetHandler = (void *)Local_u32ResetAddr;

	/* Call the Reset Handler of the use application*/
	AppResetHandler();

}
void BL_voidReceivePckt(void)
{

	uint8_t Local_u8CommandPacket[BL_MAX_PACKET_LENGTH] = {0};

	while(1)
	{
		/*Receive length to follow*/
		MUSART_u8ReceiveCharSynch(BL_USART_NUM, &Local_u8CommandPacket[0]);

		/* Receive the rest of command*/
		MUSART_u8ReceiveArraySynch(BL_USART_NUM, &Local_u8CommandPacket[1],Local_u8CommandPacket[0]);

		switch(Local_u8CommandPacket[1])

		{
		case BL_GET_VER  			:BL_voidHandle_GetVerCmd(Local_u8CommandPacket);			   	break;
		case BL_GET_HELP  			:BL_voidHandle_GetHelpCmd(Local_u8CommandPacket);		   		break;
		case BL_GET_CID  			:BL_voidHandle_GetCIDCmd(Local_u8CommandPacket);			   	break;
		case BL_GET_RDP_STATUS	    :BL_voidHandle_GetRDPStatusCmd(Local_u8CommandPacket);	   		break;
		case BL_GO_TO_ADDR          :BL_voidHandle_GoToAddressCmd(Local_u8CommandPacket);	   		break;
		case BL_FLASH_ERASE         :BL_voidHandle_FlashEraseCmd(Local_u8CommandPacket);		   	break;
		case BL_MEM_WRITE           :BL_voidHandle_MemWriteCmd(Local_u8CommandPacket);		   		break;
		case BL_MEM_RW_PROTECT      :BL_voidHandle_EnRWProtectCmd(Local_u8CommandPacket);	   		break;
		case BL_MEM_READ            :BL_voidHandle_MemReadCmd(Local_u8CommandPacket);		  	 	break;
		case BL_READ_SECTOR_STATUS  :BL_voidHandle_ReadSectorStatusCmd(Local_u8CommandPacket);  	break;
		case BL_OTP_READ            :BL_voidHandle_OTPReadCmd(Local_u8CommandPacket);		   		break;
		case BL_DIS_RW_PROTECT      :BL_voidHandle_DisRWProtectCmd(Local_u8CommandPacket);	   		break;

		}

	}

}

static uint8_t u8VerifyCRC(uint8_t *Copy_pu8DataArr, uint8_t Copy_u8Length, uint32_t Copy_u32HostCRC)
{
	uint8_t Local_u8CRCStatus,Local_u8Iterator;
	uint32_t Local_u32AccumCRC,Local_u32Temp;

	for(Local_u8Iterator = 0; Local_u8Iterator < Copy_u8Length; Local_u8Iterator++)
	{
		Local_u32Temp = Copy_pu8DataArr[Local_u8Iterator];

		Local_u32AccumCRC = CRC_u32tCalculate(&Local_u32Temp,1);

	}
	/* Reset the CRC*/
	CRC_voidReset();

	if(Local_u32AccumCRC == Copy_u32HostCRC)
	{
		Local_u8CRCStatus = CRC_SUCCESS;
	}

	else
	{
		Local_u8CRCStatus = CRC_FAIL;
	}
	return Local_u8CRCStatus;
}

static uint8_t u8ValidateAddress(uint32_t Copy_u32Addr)
{
	uint8_t Local_u8AddrStatus;

	/* As basic implementation the address is valid when in it is in FLASH or SRAM*/
	if((Copy_u32Addr >= FLASH_START) && (Copy_u32Addr <= FLASH_END))
	{
		Local_u8AddrStatus = VALID_ADDRESS;
	}
	else if((Copy_u32Addr >= SRAM_START) && (Copy_u32Addr <= SRAM_END))
	{
		Local_u8AddrStatus = VALID_ADDRESS;
	}
	else
	{
		Local_u8AddrStatus = INVALID_ADDRESS;
	}

	return Local_u8AddrStatus;
}
static void voidSendACK(uint8_t Copy_u8ReplyLength)
{

	MUSART_u8TransmitCharSynch(BL_USART_NUM,BL_ACK);
	MUSART_u8TransmitCharSynch(BL_USART_NUM,Copy_u8ReplyLength);

}

static void voidSendNACK(void)
{
	MUSART_u8TransmitCharSynch(BL_USART_NUM,BL_NACK);

}

static uint8_t u8ExecuteFlashErase(uint8_t Copy_u8SectorNumber, uint8_t Copy_u8NumberOfSectors)
{
	uint8_t Local_ErrorStatus = OK;


	if((Copy_u8SectorNumber > FLASH_MAX_NUM_SECTROS-1) && (Copy_u8SectorNumber != FLASH_MASS_ERASE_VAL))
	{
		Local_ErrorStatus = NOK;
	}
	else if((Copy_u8SectorNumber > FLASH_MAX_NUM_SECTROS) && (Copy_u8SectorNumber != FLASH_MASS_ERASE_VAL))
	{
		Local_ErrorStatus = NOK;
	}
	else
	{
		Flash_Erase_TypeDef MyFlash;

		/* Select the erase type*/
		if(Copy_u8SectorNumber == FLASH_MASS_ERASE_VAL)
		{
			/* Erase all the flash*/
			MyFlash.EraseType = FLASH_MASS_ERASE;
		}
		else
		{
			/* Erase some sectors*/
			MyFlash.EraseType = FLASH_SECTOR_ERASE;

			/* Set the first sector to be erased*/
			MyFlash.StartSector = Copy_u8SectorNumber;

			uint8_t Local_u8RemainSectors = FLASH_MAX_NUM_SECTROS - Copy_u8SectorNumber;

			if(Copy_u8NumberOfSectors > Local_u8RemainSectors)
			{
				/* if the number of sectors is bigger than the max
				 * make it equal to the max*/
				Copy_u8NumberOfSectors = Local_u8RemainSectors;
			}
			else
			{
				/**/
			}
			/* Set the last sector to be erased */
			MyFlash.EndSector = Copy_u8SectorNumber + Copy_u8NumberOfSectors;

		}

		/* Select type of parallelism*/
		MyFlash.Parallelism = FLASH_PAR_WORD;

		/* Unlock the flash*/
		FLASH_voidUnlockFlash();

		/* Start Easing*/
		FLASH_u8Erase(&MyFlash);

		/*Lock the flash*/
		FLASH_voidLock();
	}

	return Local_ErrorStatus;

}
static uint8_t u8ExecuteMemWrite(uint32_t Copy_u32Address, uint8_t Copy_u8PayloadLength, uint8_t *Copy_pu8DataBuffer)
{
	uint8_t Local_u8ErrorStatus = OK;

	if((Copy_u32Address >= FLASH_START) && (Copy_u32Address <= FLASH_END))
	{
		/* program the flash is required*/
		uint32_t Local_u32Iterator;

		/* Unlock the flash before writing*/
		FLASH_voidUnlockFlash();

		Flash_Write_TypeDef MyProgram;
		/* start writing*/
		for(Local_u32Iterator = 0; Local_u32Iterator < Copy_u8PayloadLength; Local_u32Iterator++)
		{
			MyProgram.BaseAddress = Copy_u32Address+Local_u32Iterator;
			MyProgram.Parallelism = FLASH_PAR_BYTE;
			MyProgram.Data = Copy_pu8DataBuffer[Local_u32Iterator];

			Local_u8ErrorStatus = FLASH_u8Program(&MyProgram);
		}

		/* Lock the flash*/
		FLASH_voidLock();

	}

	else
	{
		/* program the SRAM is required*/
		uint32_t Local_u32Iterator;
		uint8_t *Local_pu8DesAddrBase = (uint8_t *)Copy_u32Address;

		/* starting the writing*/
		for(Local_u32Iterator = 0; Local_u32Iterator < Copy_u8PayloadLength; Local_u32Iterator++)
		{
			Local_pu8DesAddrBase[Local_u32Iterator] = Copy_pu8DataBuffer[Local_u32Iterator];
		}
	}


	return Local_u8ErrorStatus;
}

void BL_voidHandle_GetVerCmd(uint8_t *Copy_pu8CmdPckt)
{
	uint8_t Local_u8BLVersion, Local_u8CRCStatus, Local_u8CmdLen;

	uint32_t Local_u32HostCRC;

	/* the 1st byte already include the length to follow*/
	Local_u8CmdLen = Copy_pu8CmdPckt[0]+1;

	/* Get the CRC from command*/
	Local_u32HostCRC = *((uint32_t *)(Copy_pu8CmdPckt+Local_u8CmdLen-CRC_LENGTH));

	/* Verify the CRC*/
	Local_u8CRCStatus = u8VerifyCRC(Copy_pu8CmdPckt, (Local_u8CmdLen-CRC_LENGTH), Local_u32HostCRC);

	if(Local_u8CRCStatus == CRC_SUCCESS)
	{
		voidSendACK(1u); /* need only 1 byte to send the version*/

		/*Set the BL version*/
		Local_u8BLVersion = BL_VERSION;

		/* Send the version*/
		MUSART_u8TransmitCharSynch(BL_USART_NUM,Local_u8BLVersion);
	}

	else
	{
		voidSendNACK();
	}

}
void BL_voidHandle_GetHelpCmd(uint8_t *Copy_pu8CmdPckt)
{

	uint8_t Local_u8CRCStatus, Local_u8CmdLen;

	uint32_t Local_u32HostCRC;

	/* the 1st byte already include the length to follow*/
	Local_u8CmdLen = Copy_pu8CmdPckt[0]+1;

	/* Get the CRC from command*/
	Local_u32HostCRC = *((uint32_t *)(Copy_pu8CmdPckt+Local_u8CmdLen-CRC_LENGTH));

	/* Verify the CRC*/
	Local_u8CRCStatus = u8VerifyCRC(Copy_pu8CmdPckt, (Local_u8CmdLen-CRC_LENGTH), Local_u32HostCRC);

	if(Local_u8CRCStatus == CRC_SUCCESS)
	{
		voidSendACK(1u);


		uint8_t Local_pu8BL_Commands[] =
		{
				BL_GET_VER  ,
				BL_GET_HELP  ,
				BL_GET_CID  	,
				BL_GET_RDP_STATUS,
				BL_GO_TO_ADDR     ,
				BL_FLASH_ERASE     ,
				BL_MEM_WRITE        ,
				BL_MEM_RW_PROTECT    ,
				BL_MEM_READ           ,
				BL_READ_SECTOR_STATUS ,
				BL_OTP_READ           ,
				BL_DIS_RW_PROTECT     ,
		};

		/* Send all supported commands*/
		MUSART_u8TransmitArraySynch(BL_USART_NUM, Local_pu8BL_Commands, sizeof(Local_pu8BL_Commands));
	}

	else
	{
		voidSendNACK();
	}
}
void BL_voidHandle_GetCIDCmd(uint8_t *Copy_pu8CmdPckt)
{
	uint8_t Local_u8CRCStatus, Local_u8CmdLen;

	uint32_t Local_u32HostCRC;

	/* the 1st byte already include the length to follow*/
	Local_u8CmdLen = Copy_pu8CmdPckt[0]+1;

	/* Get the CRC from command*/
	Local_u32HostCRC = *((uint32_t *)(Copy_pu8CmdPckt+Local_u8CmdLen-CRC_LENGTH));

	/* Verify the CRC*/
	Local_u8CRCStatus = u8VerifyCRC(Copy_pu8CmdPckt, (Local_u8CmdLen-CRC_LENGTH), Local_u32HostCRC);

	if(Local_u8CRCStatus == CRC_SUCCESS)
	{
		voidSendACK(2u);

		uint16_t Local_u16DeviceID;
		/*Get the Device ID*/
		Local_u16DeviceID = DBGMCU_IDCODE_REG & IDCODE_MASK;

		/* Send the Device ID*/
		MUSART_u8TransmitArraySynch(BL_USART_NUM,(uint8_t *)&Local_u16DeviceID,2);
	}

	else
	{
		voidSendNACK();
	}
}
void BL_voidHandle_GetRDPStatusCmd(uint8_t *Copy_pu8CmdPckt)
{

	uint8_t Local_u8CRCStatus, Local_u8CmdLen;

	uint32_t Local_u32HostCRC;

	/* the 1st byte already include the length to follow*/
	Local_u8CmdLen = Copy_pu8CmdPckt[0]+1;

	/* Get the CRC from command*/
	Local_u32HostCRC = *((uint32_t *)(Copy_pu8CmdPckt+Local_u8CmdLen-CRC_LENGTH));

	/* Verify the CRC*/
	Local_u8CRCStatus = u8VerifyCRC(Copy_pu8CmdPckt, (Local_u8CmdLen-CRC_LENGTH), Local_u32HostCRC);

	if(Local_u8CRCStatus == CRC_SUCCESS)
	{
		voidSendACK(1u);

		uint8_t Local_u8RDP_Status;

		/*Get The Read Protection level */
		Local_u8RDP_Status = (uint8_t )(RDP_USER_OPION_WORD_REG >>RDP_USER_OPION_SHIFT);

		/* Send the RDP level*/
		MUSART_u8TransmitCharSynch(BL_USART_NUM,Local_u8RDP_Status);
	}

	else
	{
		voidSendNACK();
	}
}

void BL_voidHandle_GoToAddressCmd(uint8_t *Copy_pu8CmdPckt)
{

	uint8_t Local_u8CRCStatus, Local_u8CmdLen;

	uint32_t Local_u32HostCRC;

	/* the 1st byte already include the length to follow*/
	Local_u8CmdLen = Copy_pu8CmdPckt[0]+1;

	/* Get the CRC from command*/
	Local_u32HostCRC = *((uint32_t *)(Copy_pu8CmdPckt+Local_u8CmdLen-CRC_LENGTH));

	/* Verify the CRC*/
	Local_u8CRCStatus = u8VerifyCRC(Copy_pu8CmdPckt, (Local_u8CmdLen-CRC_LENGTH), Local_u32HostCRC);


	if(Local_u8CRCStatus == CRC_SUCCESS)
	{
		voidSendACK(1u);

		uint32_t Local_u32DesiredAddr;
		uint8_t Local_u8AddrValidStatus;

		/* Get the desired address from the command*/
		Local_u32DesiredAddr = *((uint32_t *)&Copy_pu8CmdPckt[2]);

		/*Check address validation */
		Local_u8AddrValidStatus = u8ValidateAddress(Local_u32DesiredAddr);


		/* Send address status to the host*/
		MUSART_u8TransmitCharSynch(BL_USART_NUM,Local_u8AddrValidStatus);

		if(Local_u8AddrValidStatus == VALID_ADDRESS)
		{
			void(* JumpToAddr)(void) = NULL;

			/* Set the first bit of the address to set T bit*/
			Local_u32DesiredAddr |= 1;

			/* Assign the desired address to a function pointer*/
			JumpToAddr = (void *)Local_u32DesiredAddr;

			/* Jump to the desired address*/
			JumpToAddr();

		}
		else
		{
			/*Address is not valid*/
		}
	}

	else
	{
		voidSendNACK();
	}
}

void BL_voidHandle_FlashEraseCmd(uint8_t *Copy_pu8CmdPckt)
{

	uint8_t Local_u8CRCStatus, Local_u8CmdLen;

	uint32_t Local_u32HostCRC;

	/* the 1st byte already include the length to follow*/
	Local_u8CmdLen = Copy_pu8CmdPckt[0]+1;

	/* Get the CRC from command*/
	Local_u32HostCRC = *((uint32_t *)(Copy_pu8CmdPckt+Local_u8CmdLen-CRC_LENGTH));

	/* Verify the CRC*/
	Local_u8CRCStatus = u8VerifyCRC(Copy_pu8CmdPckt, (Local_u8CmdLen-CRC_LENGTH), Local_u32HostCRC);

	if(Local_u8CRCStatus == CRC_SUCCESS)
	{
		voidSendACK(1u);

		uint8_t Local_u8EraseStatus;

		uint8_t Local_u8DesiredSector,Local_u8NumberOfSectors;

		Local_u8DesiredSector = Copy_pu8CmdPckt[2];
		Local_u8NumberOfSectors = Copy_pu8CmdPckt[3];

		/* Erase the desired sectors*/
		Local_u8EraseStatus = u8ExecuteFlashErase(Local_u8DesiredSector, Local_u8NumberOfSectors);


		/* Send the Erase status*/
		MUSART_u8TransmitCharSynch(BL_USART_NUM,Local_u8EraseStatus);
	}

	else
	{
		voidSendNACK();
	}
}

void BL_voidHandle_MemWriteCmd(uint8_t *Copy_pu8CmdPckt)
{
	uint8_t Local_u8CRCStatus, Local_u8CmdLen;

	uint32_t Local_u32HostCRC;

	/* the 1st byte already include the length to follow*/
	Local_u8CmdLen = Copy_pu8CmdPckt[0]+1;

	/* Get the CRC from command*/
	Local_u32HostCRC = *((uint32_t *)(Copy_pu8CmdPckt+Local_u8CmdLen-CRC_LENGTH));

	/* Verify the CRC*/
	Local_u8CRCStatus = u8VerifyCRC(Copy_pu8CmdPckt, (Local_u8CmdLen-CRC_LENGTH), Local_u32HostCRC);

	if(Local_u8CRCStatus == CRC_SUCCESS)
	{
		voidSendACK(1u);

		uint8_t Local_u8Writing_Status,Local_u8AddrValid;

		/* Get the base address to writing start from it*/
		uint32_t Local_u32BaseAddr = *((uint32_t *)&(Copy_pu8CmdPckt[2]));

		/*Check address validation*/
		Local_u8AddrValid = u8ValidateAddress(Local_u32BaseAddr);

		if(Local_u8AddrValid == VALID_ADDRESS)
		{
			/* Get the pay-load length*/
			uint8_t Local_u8Payload = Copy_pu8CmdPckt[6];

			/* Start writing from this address*/
			Local_u8Writing_Status = u8ExecuteMemWrite(Local_u32BaseAddr, Local_u8Payload, (uint8_t *)&Copy_pu8CmdPckt[7]);
		}
		else
		{
			Local_u8Writing_Status = WRITING_FAILED;
		}

		/* Send the Writing status*/
		MUSART_u8TransmitCharSynch(BL_USART_NUM,Local_u8Writing_Status);
	}

	else
	{
		voidSendNACK();
	}
}
void BL_voidHandle_EnRWProtectCmd(uint8_t *Copy_pu8CmdPckt)
{
	uint8_t Local_u8CRCStatus, Local_u8CmdLen;

	uint32_t Local_u32HostCRC;

	/* the 1st byte already include the length to follow*/
	Local_u8CmdLen = Copy_pu8CmdPckt[0]+1;

	/* Get the CRC from command*/
	Local_u32HostCRC = *((uint32_t *)(Copy_pu8CmdPckt+Local_u8CmdLen-CRC_LENGTH));

	/* Verify the CRC*/
	Local_u8CRCStatus = u8VerifyCRC(Copy_pu8CmdPckt, (Local_u8CmdLen-CRC_LENGTH), Local_u32HostCRC);

	if(Local_u8CRCStatus == CRC_SUCCESS)
	{
		uint8_t Local_u8ProtectStatus;
		voidSendACK(1u);

		/* Execute protection */
		Local_u8ProtectStatus = Flash_u8EnRWProtection(Copy_pu8CmdPckt[2], Copy_pu8CmdPckt[3]);

		/* Send the status to the host */
		MUSART_u8TransmitCharSynch(BL_USART_NUM,Local_u8ProtectStatus);

	}

	else
	{
		voidSendNACK();
	}

}
void BL_voidHandle_DisRWProtectCmd(uint8_t *Copy_pu8CmdPckt)
{
	uint8_t Local_u8CRCStatus, Local_u8CmdLen;

	uint32_t Local_u32HostCRC;

	/* the 1st byte already include the length to follow*/
	Local_u8CmdLen = Copy_pu8CmdPckt[0]+1;

	/* Get the CRC from command*/
	Local_u32HostCRC = *((uint32_t *)(Copy_pu8CmdPckt+Local_u8CmdLen-CRC_LENGTH));

	/* Verify the CRC*/
	Local_u8CRCStatus = u8VerifyCRC(Copy_pu8CmdPckt, (Local_u8CmdLen-CRC_LENGTH), Local_u32HostCRC);

	if(Local_u8CRCStatus == CRC_SUCCESS)
	{
		uint8_t Local_u8ProtectStatus;

		/* Send ACK to the host */
		voidSendACK(1u);

		Local_u8ProtectStatus = Flash_u8DisAllRWProtection();

		/* Send status to the host */
		MUSART_u8TransmitCharSynch(BL_USART_NUM, Local_u8ProtectStatus);
	}

	else
	{
		voidSendNACK();
	}
}
void BL_voidHandle_ReadSectorStatusCmd(uint8_t *Copy_pu8CmdPckt)
{

	uint8_t Local_u8CRCStatus, Local_u8CmdLen;

	uint32_t Local_u32HostCRC;

	/* the 1st byte already include the length to follow*/
	Local_u8CmdLen = Copy_pu8CmdPckt[0]+1;

	/* Get the CRC from command*/
	Local_u32HostCRC = *((uint32_t *)(Copy_pu8CmdPckt+Local_u8CmdLen-CRC_LENGTH));

	/* Verify the CRC*/
	Local_u8CRCStatus = u8VerifyCRC(Copy_pu8CmdPckt, (Local_u8CmdLen-CRC_LENGTH), Local_u32HostCRC);

	if(Local_u8CRCStatus == CRC_SUCCESS)
	{
		uint8_t Local_u8ProtectStatus;

		/* Send ACK to the host */
		voidSendACK(1u);

		Local_u8ProtectStatus = Flash_u8GetProtecStatus();

		/* Send status to the host */
		MUSART_u8TransmitCharSynch(BL_USART_NUM, Local_u8ProtectStatus);
	}

	else
	{
		voidSendNACK();
	}
}
void BL_voidHandle_MemReadCmd(uint8_t *Copy_pu8CmdPckt)
{

}
void BL_voidHandle_OTPReadCmd(uint8_t *Copy_pu8CmdPckt)
{

}
