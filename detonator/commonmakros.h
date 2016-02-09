#ifndef MY_COMMON_MAKROS_H
#define MY_COMMON_MAKROS_H


#define INVBIT(port, bit) port = port ^ (1<<bit);
#define UPBIT(port, bit) port = port | (1<<bit);
#define DOWNBIT(port, bit) port = port & (~(1<<bit));

#define BITISON(reg, bit) ((reg & (1 << bit)) > 0)
#define BITISOFF(reg, bit) ((reg & (1 << bit)) == 0)


enum BOOLEAN {FALSE = 0, TRUE = 1};
#define INVBOOL(value) value = value == TRUE ? FALSE : TRUE;

#endif
