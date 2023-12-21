#ifndef INC_BL_PRV_H_
#define INC_BL_PRV_H_


#define FLASH_SECTOR_2_BASE_ADDR		0x08008000U
#define FLASH_MAX_NUM_SECTROS			8U
#define FLASH_MASS_ERASE_VAL			0xff

#define CRC_LENGTH				4u
#define CRC_SUCCESS				1u
#define CRC_FAIL				2u

#define BL_VERSION				1u


#define WRITING_SUCCESS			0U
#define WRITING_FAILED			1U

#define DBGMCU_IDCODE_REG		*((volatile uint32_t *)0xE0042000)
#define IDCODE_MASK				0x0fff


#define RDP_USER_OPION_WORD_REG		*((volatile uint32_t *)0x1FFFC000)
#define RDP_USER_OPION_SHIFT		8U


typedef enum
{

	VALID_ADDRESS,
	INVALID_ADDRESS

}BL_AddrValid_T;


static uint8_t u8VerifyCRC(uint8_t *Copy_pu8DataArr, uint8_t Copy_u8Length, uint32_t Copy_u32HostCRC);

static void voidSendACK(uint8_t Copy_u8ReplyLength);

static void voidSendNACK(void);

static uint8_t u8ValidateAddress(uint32_t Copy_u32Addr);

static uint8_t u8ExecuteFlashErase(uint8_t Copy_u8SectorNumber, uint8_t Copy_u8NumberOfSectors);

static uint8_t u8ExecuteMemWrite(uint32_t Copy_u32Address, uint8_t Copy_u8PayloadLength, uint8_t *Copy_pu8DataBuffer);

#endif /* INC_BL_PRV_H_ */
