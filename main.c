#include "led.h"
#include "keyboard.h"
#include "timer.h"

int main() {
    LedInit();
    InitTimer0(); //włączamy tylko timer, nie waidomo na ile (tak jak w punkcie 2 gdzie ustawiamy sobie InitTimer0Match0(1000000))
    while (1) 
    {
        WaitOnTimer0(1000000); // tutaj ustawiamy czas (w 2 ta funkcja nic nie przyjmuje) - tutaj ręcznie mówimy licznikowi żeby się zerował i stał aż nie naliczy 1 sekundki
        LedStepRight();
    }
}
