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
		bc_printA("r");
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
		bc_printA("r");
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
