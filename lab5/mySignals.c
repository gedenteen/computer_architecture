#include "../lab1/mySimpleComputer.h"
#include "../lab4/myReadKey.h"
#include "mySignals.h"

int ms_signalhandler(int msignal)
{
    int value;
    sc_regGet (IGNORING_CLOCK_PULSES, &value);
    if (!value)
    {
        if (msignal == SIGALRM) //SIGALRM - сигнал таймера
        {
            instructionCounter++;
        }
        else if (msignal == SIGUSR1) //SIGUSR1 - пользовательский сигнал
        {
            sc_memoryInit();
            sc_regInit();
            memx = memy = 0;
            instructionCounter = 0;
            accumulator = 0;
    }
    }
    return 0;
}
int ms_timer()
{

}
