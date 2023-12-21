#ifndef GPIO_INTERFACE_H_
#define GPIO_INTERFACE_H_

/****************************************************************************/

/*
 * @Port_T
 * */
typedef enum
{
    PORTA=0,
    PORTB,
    PORTC,
    PORTD,
    PORTE,
    PORTF,
    PORTG,
    PORTH

}Port_T;

/****************************************************************************/

/*
 * @Pin_T
 * */
typedef enum 
{
    PIN0=0,
    PIN1,
    PIN2,
    PIN3,
    PIN4,
    PIN5,
    PIN6,
    PIN7,
    PIN8,
    PIN9,
    PIN10,
    PIN11,
    PIN12,
    PIN13,
    PIN14,
    PIN15

}Pin_T;

typedef enum
{
    INPUT=0,
    OUTPUT,
    ALTER_FUNC,
    ANALOG

}Mode_T;

typedef enum
{
    SPEED_LOW=0,
    SPEED_MEDIUM,
    SPEED_FAST,
    SPEED_HIGH

}OutSpeed_T;

typedef enum
{
    PUSH_PULL=0,
    OPEN_DRAIN

}OutType_T;

/****************************************************************************/
/*
 * @PinVal_T
 * */
typedef enum
{
    PIN_LOW=0,
    PIN_HIGH

}PinVal_T;

typedef enum
{
    AF0=0,
    AF1,
    AF2,
    AF3,
    AF4,
    AF5,
    AF6,
    AF7,
    AF8,
    AF9,
    AF10,
    AF11,
    AF12,
    AF13,
    AF14,
    AF15

}AltFunc_T;

typedef enum
{
    NO_PULL=0,
    PULL_UP,
    PULL_DN

}PullUpDn_T;

typedef struct
{
    Port_T Port;
    Pin_T PinNum;
    Mode_T Mode;
    OutSpeed_T Speed;
    OutType_T OutType;
    PullUpDn_T PullType;
    AltFunc_T AltFunc;

}GPIO_PinConfig_T;

uint8_t GPIO_u8PinInit(const GPIO_PinConfig_T *PinConfig);

uint8_t GPIO_u8SetPinValue(Port_T Port , Pin_T PinNum , PinVal_T PinValue);

uint8_t GPIO_u8TogglePinValue(Port_T Port , Pin_T PinNum);

uint8_t GPIO_u8ReadPinValue(Port_T Port , Pin_T PinNum , PinVal_T *PinValue);

#endif
