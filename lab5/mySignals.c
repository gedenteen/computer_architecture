#include "../lab1/mySimpleComputer.h"
#include "../lab2/myTerm.h"
#include "../lab3/myBigChars.h"
#include "../lab4/myReadKey.h"
#include "mySignals.h"
#include "../course/myProcessor.h"

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

int ms_kbhit() {
    static bool inited = false;
    int left;

    if (!inited) {
        struct termios t;
        tcgetattr(0, &t);
        t.c_lflag &= ~ICANON;
        tcsetattr(0, TCSANOW, &t);
        setbuf(stdin, NULL);
        inited = true;
    }

    ioctl(0, FIONREAD, &left);

    return left;
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
        if ( ms_kbhit() )
        {
            enum keys key;
            rk_readkey(&key);
            if (key == KEY_Q)
                return 1;
            else
                ms_keyhandler(key);
        }
    }
    return 0;
}

//int ms_step()
//{
//    int command, operand;
//    int value = ram[instructionCounter];
//
//}

int ms_converte_write(int value, char *sign, int *command, int *operand)
{
    int temp = 1;
    temp = temp << 15; //одна единица на 15 бите
    temp = temp & value; //конъюнкция
    if (temp != 32768) // 2^15 = 32768, 15 бит - признак команды
        *sign = '+', printf("+");
    else
        *sign = '-', printf("-");
    temp = 128 - 1; //^0, ^1, ^2, ^3, ^4, ^5, ^6 -- 7 битов
    temp = temp << 7;
    *command = (value & temp) >> 7;
    temp = 128 - 1;
    *operand = value & temp;
    printf("%02X%02X", *command, *operand);
    return 0;
}

void ms_interface_static()
{
    mt_setbgcolor(RESET); //рамки
    bc_box(1, 1, 12, 61);
	bc_box(1, 62, 3, 85);
	bc_box(4, 62, 6, 85);
	bc_box(7, 62, 9, 85);
	bc_box(10, 62, 12, 85);
	bc_box(13, 1, 22, 46);
	bc_box(13, 47, 22, 85);

	mt_setfgcolor(YELLOW); //названия
	mt_gotoXY(1, 25);
	printf(" Memory: ");
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

	mt_setfgcolor(WHITE); //содержимое keys
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
}

int ms_interface()
{
	int i, x1 = -1, y1 = 0, command, operand;
	char sign;

	for (i = 0; i < 100; i++) {
		if (i % 10 == 0)
			x1++, y1 = 0;
		mt_gotoXY(x1 + 2, y1 * 6 + 2);
		if (x1 == memx && y1 == memy)
            mt_setbgcolor(YELLOW);
        ms_converte_write(ram[i], &sign, &command, &operand);
        mt_setbgcolor(RESET);
		y1++;
	}

	mt_setfgcolor(WHITE); //содержимое
	mt_gotoXY(2, 71);
	if (accumulator >= 0)
		printf("+");
	printf("%4.4d", accumulator);
	mt_gotoXY(5, 71);
	ms_converte_write(instructionCounter, &sign, &command, &operand);
	mt_gotoXY(8, 70);
	printf("+00 : 00");

	mt_gotoXY(11, 68); //флаги
	sc_regGet (OVERFLOW, &operand);
	if (operand)
        mt_setfgcolor(RED);
    printf(" O");
    mt_setfgcolor(RESET);
    sc_regGet (DIVISON_BY_ZERO, &operand);
    if (operand)
        mt_setfgcolor(RED);
    printf(" Z");
    mt_setfgcolor(RESET);
    sc_regGet (GOING_BEYOND_MEMORY, &operand);
    if (operand)
        mt_setfgcolor(RED);
    printf(" M");
    mt_setfgcolor(RESET);
    sc_regGet (IGNORING_CLOCK_PULSES, &operand);
    if (operand)
        mt_setfgcolor(RED);
    printf(" I");
    mt_setfgcolor(RESET);
    sc_regGet (WRONG_COMMAND, &operand);
    if (operand)
        mt_setfgcolor(RED);
    printf(" C");
    mt_setfgcolor(RESET);





    mt_gotoXY(14, 2);
    ms_converte_write(ram[memx*10+memy], &sign, &command, &operand);
    int file2 = open("../lab3/bigchars.txt", O_RDONLY); //бигчары
    int cnt = 0, arrbig[36] = {0}, big[2] = {0};
    bc_bigcharread(file2, arrbig, 18, &cnt);
    if (sign == '+')
        big[0] = arrbig[32], big[1] = arrbig[33];
    else
        big[0] = arrbig[34], big[1] = arrbig[35];
    bc_printbigchar(big, 14, 2, WHITE, RESET);
    int temp = command / 16;
    big[0] = arrbig[temp*2], big[1] = arrbig[temp*2+1];
    bc_printbigchar(big, 14, 11, WHITE, RESET);
    temp = command % 16;
    big[0] = arrbig[temp*2], big[1] = arrbig[temp*2+1];
    bc_printbigchar(big, 14, 20, WHITE, RESET);
    temp = operand / 16;
    big[0] = arrbig[temp*2], big[1] = arrbig[temp*2+1];
    bc_printbigchar(big, 14, 29, WHITE, RESET);
    temp = operand % 16;
    big[0] = arrbig[temp*2], big[1] = arrbig[temp*2+1];
    bc_printbigchar(big, 14, 38, WHITE, RESET);

	mt_gotoXY(23, 1);
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
                memy--, instructionCounter--;
            break;
        case KEY_RIGHT:
            if (memy < 9)
                memy++, instructionCounter++;
            break;
        case KEY_UP:
            if (memx > 0)
                memx--, instructionCounter -= 10;
            break;
        case KEY_DOWN:
            if (memx < 9)
                memx++, instructionCounter += 10;
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
        case KEY_T:
            CU();
            break;
        default:
            break;
    }
    return 0;
}
