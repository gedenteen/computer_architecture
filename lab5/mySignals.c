#include "../lab1/mySimpleComputer.h"
#include "../lab2/myTerm.h"
#include "../lab3/myBigChars.h"
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

int ms_run()
{
    struct itimerval nval, oval;
    signal (SIGALRM, ms_signalhandler);

    nval.it_interval.tv_sec = 1;
    nval.it_interval.tv_usec = 000;
    nval.it_value.tv_sec = 1;
    nval.it_value.tv_usec = 0;

    /* Запускаем таймер */
    setitimer (ITIMER_REAL, &nval, &oval);

    instructionCounter = 0;
    memx = memy = 0;
    //sc_regSet(IGNORING_CLOCK_PULSES, 1);
    while (instructionCounter <= 100)
    {
        ms_interface();
        pause();
        memy++;
        if (memy % 10 == 0)
        memy = 0, memx++;
        //instructionCounter++;
    }

    return 0;
}

int ms_interface()
{
	int i, x1 = -1, y1 = 0;
	bc_box(1, 1, 12, 61); //память
	mt_setfgcolor(YELLOW);
	mt_gotoXY(1, 25);
	printf(" Memory: ");
	mt_setbgcolor(RESET);
	for (i = 0; i < 100; i++) {
		if (i % 10 == 0)
			x1++, y1 = 0;
		mt_gotoXY(x1 + 2, y1 * 6 + 2);
		if (x1 == memx && y1 == memy)
            mt_setbgcolor(YELLOW);
        printf("+");
        printf("%04d", ram[i] % 10000);
		mt_setbgcolor(RESET);
		y1++;
	}
	mt_setbgcolor(RESET); //рамки
	bc_box(1, 62, 3, 85);
	bc_box(4, 62, 6, 85);
	bc_box(7, 62, 9, 85);
	bc_box(10, 62, 12, 85);
	bc_box(13, 1, 22, 46);
	bc_box(13, 47, 22, 85);
	mt_setfgcolor(YELLOW); //названия
	mt_gotoXY(1, 67);
	printf(" accumulator ");
	mt_gotoXY(4, 64);
	printf(" instructionCounter ");
	mt_gotoXY(7, 69);
	printf(" Operation ");
	mt_gotoXY(10, 71);
	printf(" Flags ");
	mt_gotoXY(13, 49);
	printf(" Keys ");
	mt_setfgcolor(WHITE); //содержимое
	mt_gotoXY(2, 71);
	if (accumulator >= 0)
		printf("+");
	printf("%4.4d", accumulator);
	mt_gotoXY(5, 71);
	if (instructionCounter >= 0)
		printf("+");
	printf("%4.4d", instructionCounter);
	mt_gotoXY(8, 70);
	printf("+00 : 00");
	mt_gotoXY(11, 70);
	printf(" O E V M ");

	mt_gotoXY(14, 48);
	printf("l - load");
	mt_gotoXY(15, 48);
	printf("s - save");
	mt_gotoXY(16, 48);
	printf("r - run");
	mt_gotoXY(17, 48);
	printf("t - step");
	mt_gotoXY(18, 48);
	printf("i - reset");
	mt_gotoXY(19, 48);
	printf("F5 - accumulator");
	mt_gotoXY(20, 48);
	printf("F6 - instructionCounter");
	mt_gotoXY(21, 48);
	printf("q - quit");

	int file2 = open("../lab3/bigchars.txt", O_RDONLY); //бигчары
	int cnt = 0, arrbig[24] = {0}, big[2] = {0};
	bc_bigcharread(file2, arrbig, 12, &cnt);
	if (instructionCounter >= 0)
		big[0] = arrbig[20], big[1] = arrbig[21];
	else
		big[0] = arrbig[22], big[1] = arrbig[23];
	bc_printbigchar(big, 14, 2, WHITE, RESET);
	int temp = instructionCounter % 10000 / 1000;
	big[0] = arrbig[temp*2], big[1] = arrbig[temp*2+1];
	bc_printbigchar(big, 14, 11, WHITE, RESET);
	temp = instructionCounter % 1000 / 100;
	big[0] = arrbig[temp*2], big[1] = arrbig[temp*2+1];
	bc_printbigchar(big, 14, 20, WHITE, RESET);
	temp = instructionCounter % 100 / 10;
	big[0] = arrbig[temp*2], big[1] = arrbig[temp*2+1];
	bc_printbigchar(big, 14, 29, WHITE, RESET);
	temp = instructionCounter % 10;
	big[0] = arrbig[temp*2], big[1] = arrbig[temp*2+1];
	bc_printbigchar(big, 14, 38, WHITE, RESET);
	mt_gotoXY(24, 1);

	return 0;
}

int ms_keyhandler(enum keys key)
{
    switch (key)
    {
        case KEY_L:
            sc_memoryLoad("file.dat");
            break;
        case KEY_S:
            sc_memorySave("file.dat");
            break;
        case KEY_LEFT:
            if (memy > 0) //это по горизонтали
                memy--;
            break;
        case KEY_RIGHT:
            if (memy < 9)
                memy++;
            break;
        case KEY_UP:
            if (memx > 0)
                memx--;
            break;
        case KEY_DOWN:
            if (memx < 9)
                memx++;
            break;
        case KEY_I:
            ms_signalhandler(SIGUSR1);
            break;
        case KEY_Q:
            return 1;
            break;
        case KEY_R:
            ms_run();
            break;
        default:
            break;
    }
    return 0;
}
