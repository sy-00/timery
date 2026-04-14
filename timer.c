#include <LPC21xx.H>
#include "timer.h"

#define mCOUNTER_ENABLE (1<<0)
#define mCOUNTER_RESET (1<<1)

void InitTimer0(void) {
    T0TCR = mCOUNTER_ENABLE;
}

void WaitOnTimer0(unsigned int uiTime) {
    T0TCR = T0TCR | mCOUNTER_RESET;
    T0TCR = T0TCR & (~mCOUNTER_RESET);
    while (T0TC <= ((uiTime) * 15)) {}
}
