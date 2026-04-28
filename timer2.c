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
    // do match register jest wpisywana wartosc do ktorej timer ma doliczyc
    // Ustawia wartość w rejestrze Match Register 0. Mnożnik 15 wynika z częstotliwości zegara (15 MHz), aby uzyskać czas w mikrosekundach.
    T0MCR |= (mRESET_ON_MR0 | mINTERRUPT_ON_MR0); //| (Suma logiczna): Pozwala połączyć dwie maski.
    // Mówimy procesorowi: „Kiedy czas minie, zrób dwie rzeczy: zresetuj licznik do zera i zapal lampkę (flagę) przerwania”.
    // mreset ustawia bit 1, minterrupt ustawia bit 0
    // jesli tomcr byl pisty po tej operacji ma 0011
    // Konfiguruje rejestr kontrolny dopasowania (Match Control Register). Ustawia flagi tak, aby przy zrównaniu się licznika z T0MR0 nastąpił reset licznika (T0TC) oraz wygenerowanie przerwania.
    T0TCR |= mCOUNTER_RESET; // wylaczenie
    // Ustawia bit resetu w Timer Control Register. Powoduje to natychmiastowe wyzerowanie licznika T0TC i preskalera.
    T0TCR &= ~mCOUNTER_RESET; // cofamy wylaczenie -> mamy reset
    //  Zdejmuje bit resetu, aby licznik mógł zacząć liczyć.
    T0TCR |= mCOUNTER_ENABLE; // włączenie
    //Uruchamia licznik (włącza odliczanie).
}

void WaitOnTimer0Match0() {
    while ((T0IR & mINTERRUPT_FLAG_MR0) == 0) {} //T0IR to rejestr flag (Interrupt Register). 
    // Operacja & (AND) działa jak filtr – wycina wszystko inne i patrzy tylko, czy bit „dzwonka” się zapalił.
    //Jeśli wynik to 0, znaczy, że jeszcze nie dzwoniło.
    // while ((T0IR & mINTERRUPT_FLAG_MR0) == 0) {} – Pętla oczekiwania (polling). Sprawdza bit przerwania w Interrupt Register. Jeśli jest 0, Timer jeszcze nie doliczył do wartości Match.
    T0IR = mINTERRUPT_FLAG_MR0; // Kasowanie: W tym procesorze, aby „zgasić lampkę” dzwonka, trzeba do niej paradoksalnie wpisać „1”. 
    // To sygnał dla sprzętu: „Odebrałem wiadomość, możesz zgasić flagę”.
    // T0IR = mINTERRUPT_FLAG_MR0; – Czyści flagę przerwania, aby umożliwić wykrycie kolejnego zdarzenia w przyszłości.
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




#include <LPC21xx.H>
#include "timer.h"

#define mCOUNTER_ENABLE (1<<0) 
#define mCOUNTER_RESET (1<<1) 
#define mRESET_ON_MR0 (1<<1)
#define mINTERRUPT_ON_MR0 (1<<0)
#define mINTERRUPT_FLAG_MR0 (1<<0)

void InitTimer0(void) {
    T0TCR = mCOUNTER_ENABLE;
}

void WaitOnTimer0(unsigned int uiTime) {
    T0TCR |= mCOUNTER_RESET;
    T0TCR &= (~mCOUNTER_RESET);
    while (T0TC < ((uiTime) * 15)) {}
}

void InitTimer0Match0(unsigned int uiDelayTime) {
    T0MR0 = uiDelayTime * 15;
    T0MCR |= (mRESET_ON_MR0 | mINTERRUPT_ON_MR0);
    T0TCR |= mCOUNTER_RESET;
    T0TCR &= ~mCOUNTER_RESET;
    T0TCR |= mCOUNTER_ENABLE;
}

void WaitOnTimer0Match0() {
    while ((T0IR & mINTERRUPT_FLAG_MR0) == 0) {}
    T0IR = mINTERRUPT_FLAG_MR0;
}












void InitTimer0(void) {
    T0TCR = mCOUNTER_ENABLE; // CEL: Aktywacja zliczania impulsów przez Timer0
}

void WaitOnTimer0(unsigned int uiTime) {
    T0TCR |= mCOUNTER_RESET;    // CEL: Wymuszenie stanu zerowego licznika przed startem odliczania
    T0TCR &= (~mCOUNTER_RESET); // CEL: Zwolnienie blokady resetu, aby licznik mógł ruszyć
    while (T0TC < ((uiTime) * 15)) {} // CEL: Oczekiwanie (pętla jałowa) aż licznik osiągnie limit
}

void InitTimer0Match0(unsigned int uiDelayTime) {
    T0MR0 = uiDelayTime * 15;   // CEL: Zdefiniowanie konkretnej wartości (punktu), przy której wystąpi zdarzenie
    
    // CEL: Konfiguracja zachowania sprzetu - automatyczny reset i flaga po dopasowaniu
    T0MCR |= (mRESET_ON_MR0 | mINTERRUPT_ON_MR0); 
    
    T0TCR |= mCOUNTER_RESET;    // CEL: Synchronizacja - upewnienie się, że liczymy od zera
    T0TCR &= ~mCOUNTER_RESET;   // CEL: Zakończenie fazy resetowania (przygotowanie do pracy)
    T0TCR |= mCOUNTER_ENABLE;   // CEL: Uruchomienie mechanizmu odliczania czasu
}

void WaitOnTimer0Match0() {
    // CEL: Monitorowanie flagi przerwania sprzętowego (sprawdzanie czy czas upłynął)
    while ((T0IR & mINTERRUPT_FLAG_MR0) == 0) {} 
    
    // CEL: Potwierdzenie obsługi przerwania (skasowanie flagi), by móc wykryć kolejne dopasowanie
    T0IR = mINTERRUPT_FLAG_MR0; 
}















































#include <LPC21xx.H>
#include "timer.h"

// Definicje masek bitowych
#define mCOUNTER_ENABLE (1<<0)      // Rejestr T0TCR, bit 0: Włącznik licznika
#define mCOUNTER_RESET (1<<1)       // Rejestr T0TCR, bit 1: Resetuje licznik
#define mRESET_ON_MR0 (1<<1)        // Rejestr T0MCR, bit 1: Automatyczny reset TC
#define mINTERRUPT_ON_MR0 (1<<0)    // Rejestr T0MCR, bit 0: Generuj przerwanie
#define mINTERRUPT_FLAG_MR0 (1<<0)  // Rejestr T0IR, bit 0: Flaga przerwania

void InitTimer0(void) {
    // T0TCR: Ustawienie bitu 0 - włączenie timera
    T0TCR = mCOUNTER_ENABLE;
}

void WaitOnTimer0(unsigned int uiTime) {
    // T0TCR: Ustawienie bitu 1 - reset licznika
    T0TCR |= mCOUNTER_RESET;
    // T0TCR: Czyszczenie bitu 1 - zwolnienie resetu
    T0TCR &= (~mCOUNTER_RESET);
    // Pętla czekająca, aż T0TC osiągnie zadaną wartość
    while (T0TC < ((uiTime) * 15)) {}
}

void InitTimer0Match0(unsigned int uiDelayTime) {
    // Ustawienie wartości dopasowania (Match Register 0)
    T0MR0 = uiDelayTime * 15;
    // T0MCR: Konfiguracja MR0: reset i przerwanie
    T0MCR |= (mRESET_ON_MR0 | mINTERRUPT_ON_MR0);
    // T0TCR: Reset licznika przed startem
    T0TCR |= mCOUNTER_RESET;
    // T0TCR: Zwolnienie resetu
    T0TCR &= ~mCOUNTER_RESET;
    // T0TCR: Uruchomienie timera
    T0TCR |= mCOUNTER_ENABLE;
}

void WaitOnTimer0Match0() {
    // Czekanie na zapalenie flagi przerwania (bit 0 w T0IR)
    while ((T0IR & mINTERRUPT_FLAG_MR0) == 0) {}
    // T0IR: Gasi flagę przerwania (zapisanie 1 do czyszczenia)
    T0IR = mINTERRUPT_FLAG_MR0;
}
