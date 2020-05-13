#include "../lab1/mySimpleComputer.h"
#include "../lab2/myTerm.h"
#include "../lab3/myBigChars.h"
#include "../lab4/myReadKey.h"
#include "mySignals.h"

int main()
{
    struct itimerval nval, oval;

    signal (SIGALRM, ms_signalhandler);
    //ms_signalhandler(SIGALRM);

    nval.it_interval.tv_sec = 3;
    nval.it_interval.tv_usec = 0;
    nval.it_value.tv_sec = 1;
    nval.it_value.tv_usec = 0;

    /* Запускаем таймер */
    setitimer (ITIMER_REAL, &nval, &oval);

    sc_memoryInit();
    sc_regInit();
    int i = 0;
    mt_clrscr();
    while (i != 6){
        rk_interface();
        pause();
        i++;
    }
    ms_signalhandler(SIGUSR1);
    setitimer (ITIMER_REAL, &nval, &oval);
    rk_interface();
    pause();

    return (0);
}


