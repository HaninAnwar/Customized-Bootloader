#ifndef FLASH_INTERFACE_H_
#define FLASH_INTERFACE_H_

typedef enum
{
	FLASH_SECTOR_ERASE,
	FLASH_MASS_ERASE
}Flash_EraseType;

typedef enum
{
	FLASH_PAR_BYTE,				/*Range1 : 1.7v to 2.1v*/
	FLASH_PAR_HWORD,			/*Range2 : 2.1v to 2.7v*/
	FLASH_PAR_WORD,				/*Range3 : 2.7v to 3.6v*/
	FLASH_PAR_DWORD				/*Range4 : 2.7v to 3.6v with external voltage source*/
}Flash_Parallelism;

typedef enum
{
	FLASH_SECTOR_0,
	FLASH_SECTOR_1,
	FLASH_SECTOR_2,
	FLASH_SECTOR_3,
	FLASH_SECTOR_4,
	FLASH_SECTOR_5,
	FLASH_SECTOR_6,
	FLASH_SECTOR_7

}Flash_SectorsSelect;

typedef struct
{
	Flash_EraseType EraseType;
	Flash_Parallelism Parallelism;
	Flash_SectorsSelect	StartSector;
	Flash_SectorsSelect	EndSector;

}Flash_Erase_TypeDef;

typedef struct
{
	uint32_t BaseAddress;
	Flash_Parallelism Parallelism;
	uint64_t Data;

}Flash_Write_TypeDef;

void FLASH_voidLock(void);

void FLASH_voidUnlockFlash(void);

uint8_t FLASH_u8IsBusy(void);

uint8_t FLASH_u8Erase(Flash_Erase_TypeDef *Copy_FlashErase);

uint8_t FLASH_u8Program(Flash_Write_TypeDef *Copy_FlashProgram);

void Flash_voidUnlockOTP(void);

void Flash_voidLockOTP(void);

uint8_t Flash_u8EnRWProtection(uint8_t Copy_u8ProtectSectors, uint8_t Copy_u8ProtectMode);

uint8_t Flash_u8DisAllRWProtection(void);

uint8_t Flash_u8GetProtecStatus(void);
#endif
