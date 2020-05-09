#include "../lab1/mySimpleComputer.h"
#include "../lab2/myTerm.h"
#include "myBigChars.h"

int bc_printA(char *str)
{
	printf("\E(0");
	printf("%s\n", str);
	printf("\E(B");
	return 0;
}
int bc_box(int x1, int y1, int x2, int y2) 
{
	/*
	"левый верхний угол располагается в строке x1 и столбце
	y1, а еѐ ширина и высота равна y2 столбцов и x2 строк;"
	*/
	if (x1 < 1 || y1 < 1 || x2 < 1 || y2 < 1 ||
	x2 <= x1 || y2 <= y1)
	{
		printf("Ошибка: заданы неправильные параметры для bc_box()\n");
		return -1;	
	}
	mt_gotoXY(x1, y1);
	int i;
	bc_printA("l"); //левый верхний угол
	for (i = y1 + 1; i <= y2 - 1; i++)
	{
		mt_gotoXY(x1, i);
		bc_printA("q");
	}
	mt_gotoXY(x1, y2);
	bc_printA("k"); //правый верхний
	for (i = x1 + 1; i <= x2 - 1; i++)
	{
		mt_gotoXY(i, y1);//"Первый параметр номер строки, второй - номер столбца;"
		bc_printA("x");
		mt_gotoXY(i, y2);
		bc_printA("x");
	} 
	mt_gotoXY(x2, y1);
	bc_printA("m"); //левый нижний
	for (i = y1 + 1; i <= y2 - 1; i++)
	{
		mt_gotoXY(x2, i);
		bc_printA("q");
	}
	mt_gotoXY(x2, y2);
	bc_printA("j"); //правый нижний*/
	return 0;
}
int bc_setbigcharpos (int * big, int x, int y, int value)
{
	if (x < 0 || x > 7 || y < 0 || y > 7 ||
	value < 0 || value > 1)
	{
		printf("Ошибка: заданы неправильные параметры для bc_setbigcharpos()\n");
		return -1;	
	}
	if (value)
	{
		if (x < 4)
		{
			value = 1 << (x * 8 + y);
			big[0] = big[0] | value;
		}
		else 
		{
			x -= 4;
			value = 1 << (x * 8 + y);
			big[1] = big[1] | value;
		}
	}
	else //value == 0
	{
		if (x < 4)
		{
			value = 1 << (x * 8 + y);
			value = ~value; //побитовая инверсия
			big[0] = big[0] & value;
		}
		else 
		{
			x -= 4;
			value = 1 << (x * 8 + y);
			value = ~value; //побитовая инверсия
			big[0] = big[0] & value;
		}
	}
	return 0;
}
int bc_getbigcharpos(int * big, int x, int y, int *value)
{
	if (x < 0 || x > 7 || y < 0 || y > 7)
	{
		printf("Ошибка: заданы неправильные параметры для bc_getbigcharpos()\n");
		return -1;	
	}
	if (x < 4)
	{
		*value = (big[0] >> (x * 8 + y)) & 1;
	}
	else 
	{
		x -= 4;
		*value = (big[1] >> (x * 8 + y)) & 1;
	}
	return 0;
}
int bc_printbigchar (int big[2], int x, int y, enum colors fclr, enum colors bclr)
{ //"в строке x и столбце y"
	int qew = 2147483647;
   	if (qew != 2147483647)
   	{
   		printf("Ошибка: int не четырехбитовый \n");
   		return -1;
   	}
   	if (x < 0 || y < 0)
	{
		printf("Ошибка: заданы неправильные параметры для bc_printbigchar()\n");
		return -1;	
	}
	mt_setbgcolor(bclr);
	mt_setfgcolor(fclr);
	int i, u, value;
	for (i = 0; i < 8; i++)
	{
		for (u = 0; u < 8; u++)
		{
			bc_getbigcharpos(big, i, u, &value);
			mt_gotoXY(x + i, y + u);
			if (value)
				bc_printA("a");
			else
				bc_printA(" ");
		}
	}
	mt_setbgcolor(RESET);
	mt_setfgcolor(WHITE);
	return 0;
}
int bc_bigcharwrite (int fd, int * big, int count)
{
	for (int i = 0; i < count * 2; i++)
		if (write(fd, &big[i], sizeof(int)) == -1)
			return -1;
	return 0;
}
int bc_bigcharread (int fd, int * big, int need_count, int * count)
{
    for (*count = 0; *count < need_count * 2; *count += 1)
        if (read(fd, &big[*count], sizeof(int)) == -1)
            return -1;
    return 0;
}
int bc_showinterface()
{
	int i, x1 = 1, y1 = 2; 
	bc_box(1, 1, 12, 61); //память
	mt_setfgcolor(YELLOW);
	mt_gotoXY(1, 25);
	printf(" Memory: ");
	mt_setbgcolor(RESET);
	mt_setfgcolor(WHITE);
	for (i = 0; i < 100; i++) {
		if (i % 10 == 0)
			x1++, y1 = 2;
		mt_gotoXY(x1, y1);
		if (ram[i] < 0)
			mt_setbgcolor(BLUE);
		if (ram[i] > 0)	
			mt_setbgcolor(RED);
		if (ram[i] >= 0)
			printf("+"); 
		printf("%4.4d", ram[i]);
		mt_setbgcolor(RESET);
		y1 += 6;
	}
	mt_setbgcolor(RESET); //рамки
	bc_box(1, 62, 3, 85); 
	bc_box(4, 62, 6, 85); 
	bc_box(7, 62, 9, 85); 
	bc_box(10, 62, 12, 85);
	bc_box(13, 1, 23, 46); 
	bc_box(13, 47, 23, 85);
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
	int accum = 0, instcnt = 1; //содержимое
	mt_setfgcolor(WHITE);
	mt_gotoXY(2, 71);
	if (accum >= 0)
		printf("+");
	printf("%4.4d", accum);
	mt_gotoXY(5, 71);
	if (accum >= 0)
		printf("+");
	printf("%4.4d", instcnt);
	mt_gotoXY(8, 70);
	printf("+00 : 00");
	mt_gotoXY(11, 70);
	printf(" O E V M ");

	int file2 = open("bigchars.txt", O_RDONLY); //бигчары
	int cnt = 0, arrbig[24] = {0}, big[2] = {0};
	bc_bigcharread(file2, arrbig, 12, &cnt);
	if (instcnt >= 0)
		big[0] = arrbig[20], big[1] = arrbig[21];
	else
		big[0] = arrbig[22], big[1] = arrbig[23];
	bc_printbigchar(big, 14, 2, WHITE, RESET);
	int temp = instcnt % 10000 / 1000;
	big[0] = arrbig[temp*2], big[1] = arrbig[temp*2+1];
	bc_printbigchar(big, 14, 11, WHITE, RESET);
	temp = instcnt % 1000 / 100;
	big[0] = arrbig[temp*2], big[1] = arrbig[temp*2+1];
	bc_printbigchar(big, 14, 20, WHITE, RESET);
	temp = instcnt % 100 / 10;
	big[0] = arrbig[temp*2], big[1] = arrbig[temp*2+1];
	bc_printbigchar(big, 14, 29, WHITE, RESET);
	temp = instcnt % 10;
	big[0] = arrbig[temp*2], big[1] = arrbig[temp*2+1];
	bc_printbigchar(big, 14, 38, WHITE, RESET);

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
	
	return 0;
}
