#include <LPC21xx.H>
#include "timer.h"

#define mCOUNTER_ENABLE (1<<0)
#define mCOUNTER_RESET (1<<1)

// tutaj nie ma funcji z timer match - sami musimy pilnować czas - nie używamy bloku porównującego

void InitTimer0(void) {
    T0TCR = mCOUNTER_ENABLE;
}

void WaitOnTimer0(unsigned int uiTime) {
    T0TCR = T0TCR | mCOUNTER_RESET; //włączamy reset
    T0TCR = T0TCR & (~mCOUNTER_RESET); // wyłącamy reset
    // czyli ręcznie zerujemy licznik żeby mógł ruszyć od zera
    while (T0TC <= ((uiTime) * 15)) {} // cały czas sprawdzamy licznik (T0TC) i porównujemy z wyliczoną wartością 
    // to taj jakby co mikrosekundę sprawdzać czy naliczyło ich już sekundę zamiast ustawić budzik
}
