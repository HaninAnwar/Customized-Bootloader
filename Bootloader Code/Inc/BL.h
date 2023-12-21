#ifndef INC_BL_H_
#define INC_BL_H_


#define BL_MAX_PACKET_LENGTH	255U
#define BL_ACK					0xA5
#define BL_NACK					0x7F

#define BL_USART_NUM			USART_2

#define FLASH_SIZE				(512*1024)
#define SRAM_SIZE				(128*1024)

#define FLASH_START				0x08000000UL
#define FLASH_END				((FLASH_START)+(FLASH_SIZE))

#define SRAM_START				0x2000000UL
#define SRAM_END				((SRAM_START)+(SRAM_SIZE))

typedef enum
{
	BL_GET_VER  			=0X51,
	BL_GET_HELP  			=0X52,
	BL_GET_CID  			=0X53,
	BL_GET_RDP_STATUS	  	=0X54,
	BL_GO_TO_ADDR           =0X55,
	BL_FLASH_ERASE          =0X56,
	BL_MEM_WRITE            =0X57,
	BL_MEM_RW_PROTECT       =0X58,
	BL_MEM_READ             =0X59,
	BL_READ_SECTOR_STATUS   =0X5A,
	BL_OTP_READ             =0X5B,
	BL_DIS_RW_PROTECT       =0X5C
}BL_Command;

void BL_voidJumpToUserApp(void);

void BL_voidReceivePckt(void);

void BL_voidHandle_GetVerCmd(uint8_t *Copy_pu8CmdPckt);
void BL_voidHandle_GetHelpCmd(uint8_t *Copy_pu8CmdPckt);
void BL_voidHandle_GetCIDCmd(uint8_t *Copy_pu8CmdPckt);
void BL_voidHandle_GetRDPStatusCmd(uint8_t *Copy_pu8CmdPckt);
void BL_voidHandle_GoToAddressCmd(uint8_t *Copy_pu8CmdPckt);
void BL_voidHandle_FlashEraseCmd(uint8_t *Copy_pu8CmdPckt);
void BL_voidHandle_MemWriteCmd(uint8_t *Copy_pu8CmdPckt);
void BL_voidHandle_EnRWProtectCmd(uint8_t *Copy_pu8CmdPckt);
void BL_voidHandle_MemReadCmd(uint8_t *Copy_pu8CmdPckt);
void BL_voidHandle_ReadSectorStatusCmd(uint8_t *Copy_pu8CmdPckt);
void BL_voidHandle_OTPReadCmd(uint8_t *Copy_pu8CmdPckt);
void BL_voidHandle_DisRWProtectCmd(uint8_t *Copy_pu8CmdPckt);

#endif /* INC_BL_H_ */
