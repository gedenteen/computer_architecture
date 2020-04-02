#include "../lab1/mySimpleComputer.h"
#include "../lab2/myTerm.h"
#include <stdio.h>
#include <math.h>

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
int bc_printbigchar (int arr[2], int x, int y, enum colors color1, enum colors color2)
{ //"в строке x и столбце y"
	/*int qew = 2147483647;
   	if (qew != 2147483647)
   	{
   		printf("Ошибка: int не четырехбитовый \n");
   		return -1;
   	}*/
	int i, u, temp = arr[0], value;
	for (i = x; i < x + 8; i++)
	{
		if (i == x + 4)
			temp = arr[1];
		for (u = y; u < y + 8; u++)
		{
			value = temp & 1;
			temp = temp >> 1;
			if (value)
			{
				mt_gotoXY(i, u);
				bc_printA("a");
			}
		}
	
	}
	
	return 0;
}
////////////////
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
   	
   	
   	/*mt_gotoXY(12,1);
   	mt_setfgcolor(GREEN);
   	printf("1 ");
   	printf("\E(0");
   	printf("aaaaaaaa");
   	printf("\E(B \n");	
   	printf("2 ");
   	printf("\E(0");
   	printf("aa    aa");
   	printf("\E(B \n");
   	printf("3 ");
   	printf("\E(0");
   	printf("aa    aa");
   	printf("\E(B \n");
   	printf("4 ");
   	printf("\E(0");
   	printf("aa    aa");
   	printf("\E(B \n");	
   	printf("5 ");
   	printf("\E(0");
   	printf("aaaaaaaa");
   	printf("\E(B \n");
   	printf("6 ");
   	printf("\E(0");
   	printf("      aa");
   	printf("\E(B \n");
   	printf("7 ");
   	printf("\E(0");
   	printf("      aa");
   	printf("\E(B \n");
   	printf("1 ");
   	printf("\E(0");
   	printf("aaaaaaaa");
   	printf("\E(B \n");	
   	printf("  12345678");*/
   	
   	
   	
    
    printf("\n");
    return 0;
}

