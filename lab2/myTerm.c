#include "myTerm.h"
#include "../lab1/mySimpleComputer.h"

int mt_clrscr(void)
{
	printf("\E[2J");
	printf("\e[1;1H");
	return 0;
}


int mt_getscreensize (int * rows, int * cols)
{
	struct winsize w;
	if ( !ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) ) 
	{
		*rows = w.ws_row;
		*cols = w.ws_col;
		return 0;
	} else
	{
		printf("Ошибка при выполнении команды ioctl \n");
		return -1;
	} 

}

int mt_gotoXY (int y, int x)
{
	int rows, cols;
	mt_getscreensize (&rows, &cols);
	if (y > rows || x > cols)
	{
		printf("Ошибка: указана строка и/или столбец, превышающие размер(%d, %d) терминала\n", x, y);
		return -1;	
	}	

	printf("\e[%d;%dH", y, x);
	
	return 0;
}

int mt_setfgcolor (enum colors color)
{
	if (color == RESET)
		printf("\e[0m");
	printf("\e[3%dm", color);
	return 0;
}

int mt_setbgcolor (enum colors color)
{
	if (color == RESET)
		printf("\e[0m");
	printf("\e[4%dm", color);
	return 0;
}

int mt_showinterface ()
{
	int i;
	mt_setfgcolor(YELLOW);
	printf("\t Memory: \n");
	mt_setbgcolor(GRAY);
	mt_setfgcolor(WHITE);
	for (i = 0; i < 100; i++) {
		if (ram[i] < 0)
			mt_setbgcolor(BLUE);
		if (ram[i] > 0)	
			mt_setbgcolor(RED);
		if (ram[i] >= 0)
			printf("+"); 
		printf("%4.4d ", ram[i]);
		if ((i + 1) % 10 == 0)
			printf("\n");
		mt_setbgcolor(GRAY);
	}
	mt_setbgcolor(RESET);
	mt_setfgcolor(YELLOW);
	printf("\n Keys: \n");
	mt_setfgcolor(WHITE);
	printf(" l  - load \n");
	printf(" s  - save \n");
	printf(" r  - run \n");
	printf(" t  - step \n");
	printf(" i  - reset \n");
	printf(" F5 - accumulator \n");
	printf(" F6 - instructionCounter \n");
	mt_setfgcolor(YELLOW);
	mt_gotoXY(18, 30); printf("Registers: \n");
	mt_setfgcolor(WHITE);
	mt_gotoXY(19, 30); printf("OVERFLOW == 1 \n");
	mt_gotoXY(20, 30); printf("DIVISON_BY_ZERO == 2 \n");
	mt_gotoXY(21, 30); printf("GOING_BEYOND_MEMORY == 3 \n");
	mt_gotoXY(22, 30); printf("GNORING_CLOCK_PULSES == 4 \n");
	mt_gotoXY(23, 30); printf("WRONG_COMMAND == 5 \n");
	mt_gotoXY(25, 0);
	
	return 0;
}
