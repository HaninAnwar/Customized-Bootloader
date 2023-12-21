
#ifndef BIT_MATH_H
#define BIT_MATH_H



#define SET_BIT(REG , BITNUM) REG |= 0b1<<BITNUM
#define CLR_BIT(REG , BITNUM) REG &= ~(0b1<<BITNUM)
#define TOG_BIT(REG , BITNUM) REG ^= 0b1<<BITNUM
#define GET_BIT(REG , BITNUM) ( (REG >> BITNUM) & 0b1 )





#endif
