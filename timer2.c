#include <LPC21xx.H>
#include "timer.h"
// usuwamy odpytywanie (polling) gdzie procesor był skupiony na pilnowaniu czasu
// teraz procesor zleca pomiar czasu timerowi

#define mCOUNTER_ENABLE (1<<0) 
// To binarnie: 00000001 (Jedynka na pozycji 0)
#define mCOUNTER_RESET (1<<1) 
// To binarnie: 00000010 (Jedynka na pozycji 1)
#define mRESET_ON_MR0 (1<<1)
#define mINTERRUPT_ON_MR0 (1<<0)
#define mINTERRUPT_FLAG_MR0 (1<<0)

void InitTimer0(void) {
    T0TCR = mCOUNTER_ENABLE;  // T0TCR: To rejestr sterujący Timerem 0 (Timer Control Register).
}

void WaitOnTimer0(unsigned int uiTime) {
    T0TCR |= mCOUNTER_RESET; //or - włacza bit bez wpływania na inne (or - przynajmniej jedno 1)
    T0TCR &= (~mCOUNTER_RESET); //wyłączdenie bitu (and z negacja - samo end potrzebuje obydwu jedynek)
    while (T0TC < ((uiTime) * 15)) {} //T0TC to rejestr ktory sam sie zwieksza
    // Mnożymy czas przez 15 (bo 15 impulsów to 1 mikrosekunda)
    // Procesor kręci się w while, dopóki T0TC nie dobije do tej wartości (1 mikrosekundy)
}

void InitTimer0Match0(unsigned int uiDelayTime) {
    T0MR0 = uiDelayTime * 15; // Wpisujemy do "budzika" (Match Register) czas
    T0MCR |= (mRESET_ON_MR0 | mINTERRUPT_ON_MR0); //| (Suma logiczna): Pozwala połączyć dwie maski.
    // Mówimy procesorowi: „Kiedy czas minie, zrób dwie rzeczy: zresetuj licznik do zera i zapal lampkę (flagę) przerwania”.
    T0TCR |= mCOUNTER_RESET;
    T0TCR &= ~mCOUNTER_RESET;
    T0TCR |= mCOUNTER_ENABLE;
}

void WaitOnTimer0Match0() {
    while ((T0IR & mINTERRUPT_FLAG_MR0) == 0) {} //T0IR to rejestr flag (Interrupt Register). 
    // Operacja & (AND) działa jak filtr – wycina wszystko inne i patrzy tylko, czy bit „dzwonka” się zapalił.
    //Jeśli wynik to 0, znaczy, że jeszcze nie dzwoniło.
    T0IR = mINTERRUPT_FLAG_MR0; // Kasowanie: W tym procesorze, aby „zgasić lampkę” dzwonka, trzeba do niej paradoksalnie wpisać „1”. 
    // To sygnał dla sprzętu: „Odebrałem wiadomość, możesz zgasić flagę”.
}



// rzeczy z T0: rejestry przypisane do timera 0 (każdy po 32 bity)

// T0TCR - Timer Control Register - obslugujemy z apomoca mCounterEnable
// - na najniższej pozycji 1 -> timer dziala
// - na 2 pozycji 1 -> timer nie dziala

// T0TC
// tutaj timer 15 milionów razy na sekundę dorzuca +1
// to aktualny czas stopera

// T0MR0 - Match Register 0
// tutaj wpisujemy liczbę przy której ma się coś stać
// jeśli wpiszemy 1500 to po 1500 impulskie (100 mikrosekundach) coś się stanie

// T0MCR - Match Control Register
// tuttaj mówimy co ma się stać gdy dobijemy do MR0
// resetujemy do zera (żeby zaczął liczyć od nowa i były równe odstępy czasu)
// zapalamy lampke powiadomienia

// T0IR - Interrupt Register
// lampka powiadomienia - flaga
// w while jeśli flaga leży (0) to czekamy, jeżeli podniesiona (1) to czas minął!!!111!!!!!@#@!2@!!111!!




// przeliczanie na mikrosekundy z impulsów i coś tam:

// procesor działa z 60MHz (60 mln. razy na sekundę)
// zegar peryferiów jest 4 razy wolniejszy - 15MHz (15 mln. razy na sekundę)
// => timer w ciągu sekundy naliczy 15 mln. impulsów

// ile impulsów przypada na 1 mikto sekundę?
// 1 sekunda to 1 mln. mikrosekund
// impulsy/mikrosekundy = 15mln/1mln = 15
// => jeśli chcemy kazać licznikowy czekać 1 mikrosekunde to każemy mu czekać 15 impulsów

