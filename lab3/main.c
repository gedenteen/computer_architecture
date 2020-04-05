#include "../lab1/mySimpleComputer.h"
#include "../lab2/myTerm.h"
#include <stdio.h>
//#include <math.h>
//#include <io.h>
#include <fcntl.h>
//#include <stdlib.h>

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
int bc_printbigchar (int big[2], int x, int y, enum colors color1, enum colors color2)
{ //"в строке x и столбце y"
	/*int qew = 2147483647;
   	if (qew != 2147483647)
   	{
   		printf("Ошибка: int не четырехбитовый \n");
   		return -1;
   	}*/
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

///////////////
int main()
{
    //команда в терминале: man console_codes
    printf("\E(0");
    printf("1q 2w 3e 4r 6t 7y 8u 9i 10o 11p [ ] 2 \n");
    printf("1a 2s 3d 4f 5g 6h 7j 8k 9l ; ' \n");
    printf("lqqqqqqqk\n");
    printf("lrrrrrrrrrrrrrk\n");
    printf("looooooooook\n");
    printf("lpppppppk\n");
    printf("1z 2x 3c 4v 5b 6n 7m 8, 9<\n\n");
    printf("\E(B");

    mt_clrscr();
    bc_box(1, 3, 11, 70);

    int arr[2];
    arr[0] = 1 + 2 + 64 + 128;
    arr[0] <<=  8;
    arr[0] += 1 + 2 + 64 + 128;
    arr[0] <<= 8;
    arr[0] += 1 + 2 + 64 + 128;
    arr[0] <<= 8;
    arr[0] += 256 - 1;
    //arr[0] += pow(2, 24) + pow(2,25) + pow(2, 30) + pow (2, 31);
    arr[1] = 256 - 1;
    arr[1] <<= 8;
    arr[1] += 64 + 128;
    arr[1] <<= 8;
    arr[1] += 64 + 128;
    arr[1] <<= 8;
    arr[1] += 256 - 1;
    mt_gotoXY(2,4);
    printf("12345678");
    bc_printbigchar(arr, 3, 4, WHITE, YELLOW);

    mt_gotoXY(12,4);
    bc_setbigcharpos (arr, 6, 0, 1);
    bc_setbigcharpos (arr, 0, 7, 0);
    bc_setbigcharpos (arr, 1, 7, 1);
    bc_setbigcharpos (arr, 1, 1, 0);
    bc_printbigchar(arr, 3, 14, WHITE, YELLOW);

	arr[0] = 1;	
    printf("%d %d \n", arr[0], arr[1]);
    int file = open("bigchars.txt", O_RDWR | O_CREAT);
    bc_bigcharwrite (file, arr, 1);
    int file2 = open("bigchars.txt", O_RDONLY);
    int cunt = 0, arr2[2] = {0};
    bc_bigcharread(file2, arr2, 1, &cunt);
    printf("%d %d \n", arr2[0], arr2[1]);

    printf("\n");
    return 0;
}

