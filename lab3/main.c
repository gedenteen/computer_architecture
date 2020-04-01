#include "../lab1/mySimpleComputer.h"
#include "../lab2/myTerm.h"
#include <stdio.h>

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
}
////////////////
int main()
{
	//команда в терминале: man console_codes
    
    printf("m\n");
    
    printf("\E(0");
   	printf("1q 2w 3e 4r 6t 7y 8u 9i 10o 11p [ ] 2 \n");
   	printf("1a 2s 3d 4f 5g 6h 7j 8k 9l ; ' \n");
   	printf("lqqqqqqqk\n");
   	printf("lrrrrrrrrrrrrrk\n");
   	printf("looooooooook\n");
   	printf("lpppppppk\n");
   	printf("1z 2x 3c 4v 5b 6n 7m 8, 9<\n\n");
   	
    printf("\E(B");
    
   	bc_printA("q");
   	bc_printA("Q");
   	
   	mt_clrscr();
   	bc_box(1, 3, 10, 70);
   	printf("asdad");
	/*printf("\E(0");
    printf("lrrrrrrrrrrrrrk\n");
    printf("x             x\n");
    printf("x             x\n");
    printf("x             x\n");
    printf("x             x\n");
    printf("x             x\n");
    printf("x             x\n");
    printf("mrrrrrrrrrrrrrj\n");
    printf("\E(B");*/
   	
   	
   	
    
    printf("\n");
    return 0;
}

