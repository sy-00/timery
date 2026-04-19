#include "led.h"
#include "timer.h"

int main() {
    LedInit(); //ustawienie ledów jako wyjść
    InitTimer0Match0(1000000); // budzik na 1mln mikrosekund - 1 sekundkę
    while (1) {
        WaitOnTimer0Match0(); // procesor w kółko sprawdza rejestr T0IR (flagę)
        LedStepLeft(); // co sekundkę lecimy ledem w lewo
    }
}
// czyli zobaczymy w nieskończoność zapalające się co sekundę ledy idące w lewo!!!!!!
