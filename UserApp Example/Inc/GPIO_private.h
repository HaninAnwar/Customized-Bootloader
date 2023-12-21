#ifndef GPIO_PRIVATE_H_
#define GPIO_PRIVATE_H_

#define GPIO_PERIPHERAL_NUM		8U

#define MODER_MASK				0b11
#define MODER_PIN_ACCESS		2U

#define PUPDR_MASK				0b11
#define PUPDR_PIN_ACCESS		2U

#define OTYPER_MASK				0b1

#define OSPEEDER_MASK			0b11
#define OSPEEDER_PIN_ACCESS		2U

#define AFR_MASK				0b1111
#define AFR_PIN_ACCESS			4U
#define AFR_DIV_REM				8U

#define ODR_MASK				0b1

#define IDR_ANDING				1U
#endif
