#include "led.h"
#include "keyboard.h"
#include "timer.h"

int main() {
    LedInit();
    InitTimer0();
    while (1) 
    {
        WaitOnTimer0(1000000);
        LedStepRight();
    }
}
