#include "../lab1/mySimpleComputer.h"
#include "../lab2/myTerm.h"
#include "../lab3/myBigChars.h"
#include "../lab4/myReadKey.h"
#include "mySignals.h"

int main()
{
    //ms_signalhandler(SIGUSR1);
    mt_clrscr();
    //memx = memy = 0;
    //ms_interface();

    struct itimerval nval, oval;
    signal (SIGALRM, ms_signalhandler);

    nval.it_interval.tv_sec = 1;
    nval.it_interval.tv_usec = 0;
    nval.it_value.tv_sec = 1;
    nval.it_value.tv_usec = 0;

    /* Запускаем таймер */
    //setitimer (ITIMER_REAL, &nval, &oval);

    sc_memoryInit();
    sc_regInit();
    int i = 0;
    mt_clrscr();

    ram[12] = 12;
    instructionCounter = 20;
    accumulator = 22;

    while (i != 15) {
        ms_interface();
        //pause();
        enum keys key = -1;
        rk_readkey(&key);
        if (ms_keyhandler(key) == 1)
            i = 14;
        i++;
    }

    return 0;
}


