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
    bc_box(1, 3, 21, 70);

    int arrbig[24] = {0};
    arrbig[0] = 1 + 2 + 64 + 128;
    arrbig[0] <<=  8;
    arrbig[0] += 1 + 2 + 64 + 128;
    arrbig[0] <<= 8;
    arrbig[0] += 1 + 2 + 64 + 128;
    arrbig[0] <<= 8;
    arrbig[0] += 2 + 4 + 8 + 16 + 32 + 64;
    //arr[0] += pow(2, 24) + pow(2,25) + pow(2, 30) + pow (2, 31);
    arrbig[1] = 2 + 4 + 8 + 16 + 32 + 64;
    arrbig[1] <<= 8;
    arrbig[1] += 1 + 2 + 64 + 128;
    arrbig[1] <<= 8;
    arrbig[1] += 1 + 2 + 64 + 128;
    arrbig[1] <<= 8;
    arrbig[1] += 1 + 2 + 64 + 128;
    mt_gotoXY(2,4);
    printf("12345678");
    bc_printbigchar(arrbig, 3, 4, WHITE, YELLOW);
    
    int big[2] = {0};
    arrbig[2] = 8 + 16;
    arrbig[2] <<=  8;
    arrbig[2] += 2 + 4 + 8 + 16;
    arrbig[2] <<=  8;
    arrbig[2] += 4 + 8 + 16;
    arrbig[2] <<=  8;
    arrbig[2] += 8 + 16;
    arrbig[3] = 8 + 16;
    arrbig[3] <<=  8;
    arrbig[3] += 8 + 16;
    arrbig[3] <<=  8;
    arrbig[3] += 8 + 16;
    arrbig[3] <<=  8;
    arrbig[3] += 8 + 16;
    mt_gotoXY(2, 14);
    printf("12345678");
    big[0] = arrbig[2], big[1] = arrbig[3];
    bc_printbigchar(big, 3, 14, WHITE, YELLOW);
    
    arrbig[4] = 64 + 128;
    arrbig[4] <<=  8;
    arrbig[4] += 64 + 128;
    arrbig[4] <<=  8;
    arrbig[4] += 1 +64 + 128;
    arrbig[4] <<=  8;
    arrbig[4] += 2 + 4 + 8 + 16 + 32 + 64;
    ///////////
    arrbig[5] = 1 +2 + 4 + 8 + 16 + 32 + 64  + 128;
    arrbig[5] <<=  8;
    arrbig[5] += 1+2;
    arrbig[5] <<=  8;
    arrbig[5] += 1+2;
    arrbig[5] <<=  8;
    arrbig[5] += 2+4 + 8 + 16 + 32 + 64;
    mt_gotoXY(2, 24);
    printf("12345678");
    big[0] = arrbig[4], big[1] = arrbig[5];
    bc_printbigchar(big, 3, 24, WHITE, YELLOW);
    
    arrbig[6] = 2 + 4 + 8 + 16 + 32 + 64; //aaaaaaaaaaaaaaa
    arrbig[6] <<=  8;
    arrbig[6] += 64 + 128;
    arrbig[6] <<=  8;
    arrbig[6] += 1+64 + 128;
    arrbig[6] <<=  8;
    arrbig[6] += 2 + 4 + 8 + 16 + 32 + 64;
    ///////////
    arrbig[7] = 2 + 4 + 8 + 16 + 32 + 64;
    arrbig[7] <<=  8;
    arrbig[7] += 1+64 + 128;
    arrbig[7] <<=  8;
    arrbig[7] += 64 + 128;
    arrbig[7] <<=  8;
    arrbig[7] += 64 + 128;
    mt_gotoXY(2, 34);
    printf("12345678");
    big[0] = arrbig[6], big[1] = arrbig[7];
    bc_printbigchar(big, 3, 34, WHITE, YELLOW);
    
    arrbig[8] = 1 + 2 + 4 + 8 + 16 + 32 + 64  + 128 ; //aaaaaaaaaaaaaaa
    arrbig[8] <<=  8;
    arrbig[8] += 1 +2+ 64+128 ;
    arrbig[8] <<=  8;
    arrbig[8] += 1 +2+ 64+128 ;
    arrbig[8] <<=  8;
    arrbig[8] += 1 +2+ 64+128 ;
    ///////////
    arrbig[9] = 64 + 128;
    arrbig[9] <<=  8;
    arrbig[9] += 64 + 128;
    arrbig[9] <<=  8;
    arrbig[9] += 64 + 128;
    arrbig[9] <<=  8;
    arrbig[9] += 64 + 128;
    mt_gotoXY(2, 44);
    printf("12345678");
    big[0] = arrbig[8], big[1] = arrbig[9];
    bc_printbigchar(big, 3, 44, WHITE, YELLOW);
    
    arrbig[10] = 2 + 4 + 8 + 16 + 32 + 64 ; //aaaaaaaaaaaaaaa
    arrbig[10] <<=  8;
    arrbig[10] += 1 +2;
    arrbig[10] <<=  8;
    arrbig[10] += 1 +2;
    arrbig[10] <<=  8;
    arrbig[10] += 1 + 2 + 4 + 8 + 16 + 32 + 64  + 128;
    ///////////
    arrbig[11] = 2 + 4 + 8 + 16 + 32 + 64;
    arrbig[11] <<=  8;
    arrbig[11] += 1+64 + 128;
    arrbig[11] <<=  8;
    arrbig[11] += 64 + 128;
    arrbig[11] <<=  8;
    arrbig[11] += 64 + 128;
    mt_gotoXY(2, 54);
    printf("12345678");
    big[0] = arrbig[10], big[1] = arrbig[11];
    bc_printbigchar(big, 3, 54, WHITE, YELLOW);
    
    arrbig[12] = 1+2+4+8+16+32+64 ; //aaaaaaaaaaaaaaa
    arrbig[12] <<=  8;
    arrbig[12] += 1+2;
    arrbig[12] <<=  8;
    arrbig[12] += 1+2;
    arrbig[12] <<=  8;
    arrbig[12] += 2+4+8+16+32+64;
    ///////////
    arrbig[13] = 2 + 4 + 8 + 16 + 32 + 64;
    arrbig[13] <<=  8;
    arrbig[13] += 1+2+64 + 128;
    arrbig[13] <<=  8;
    arrbig[13] += 1+2+64 + 128;
    arrbig[13] <<=  8;
    arrbig[13] += 1+2+64 + 128;
    mt_gotoXY(12, 4);
    printf("12345678");
    big[0] = arrbig[12], big[1] = arrbig[13];
    bc_printbigchar(big, 13, 4, WHITE, YELLOW);
    
    arrbig[14] = 32+64; 
    arrbig[14] <<=  8;
    arrbig[14] += 32+64;
    arrbig[14] <<=  8;
    arrbig[14] += 64+128;
    arrbig[14] <<=  8;
    arrbig[14] += 1 + 2 + 4 + 8 + 16 + 32 + 64  + 128;
    ///////////
    arrbig[15] = 8+16;
    arrbig[15] <<=  8;
    arrbig[15] += 8+16;
    arrbig[15] <<=  8;
    arrbig[15] += 16+32; 
    arrbig[15] <<=  8;
    arrbig[15] += 16+32;
    mt_gotoXY(12, 14);
    printf("12345678");
    big[0] = arrbig[14], big[1] = arrbig[15];
    bc_printbigchar(big, 13, 14, WHITE, YELLOW);
    
    arrbig[16] = 2 + 4 + 8 + 16 + 32 + 64; 
    arrbig[16] <<=  8;
    arrbig[16] += 1+2+64+128;
    arrbig[16] <<=  8;
    arrbig[16] += 1+2+64+128;
    arrbig[16] <<=  8;
    arrbig[16] += 2 + 4 + 8 + 16 + 32 + 64;
    ///////////
    arrbig[17] =  2 + 4 + 8 + 16 + 32 + 64;
    arrbig[17] <<=  8;
    arrbig[17] += 1+2+64+128;
    arrbig[17] <<=  8;
    arrbig[17] += 1+2+64+128; 
    arrbig[17] <<=  8;
    arrbig[17] += 1+2+64+128;
    mt_gotoXY(12, 24);
    printf("12345678");
    big[0] = arrbig[16], big[1] = arrbig[17];
    bc_printbigchar(big, 13, 24, WHITE, YELLOW);
    
    arrbig[18] = 1+2+64+128; 
    arrbig[18] <<=  8;
    arrbig[18] += 1+2+64+128;
    arrbig[18] <<=  8;
    arrbig[18] += 1+2+64+128;
    arrbig[18] <<=  8;
    arrbig[18] += 2 + 4 + 8 + 16 + 32 + 64; 
    ///////////
    arrbig[19] =  2 + 4 + 8 + 16 + 32 + 64; 
    arrbig[19] <<=  8;
    arrbig[19] += 64+128;
    arrbig[19] <<=  8;
    arrbig[19] += 64+128; 
    arrbig[19] <<=  8;
    arrbig[19] += 2 + 4 + 8 + 16 + 32 + 64+128;
    mt_gotoXY(12, 34);
    printf("12345678");
    big[0] = arrbig[18], big[1] = arrbig[19];
    bc_printbigchar(big, 13, 34, WHITE, YELLOW);
    
    arrbig[20] = 1+2 + 4 + 8 + 16 + 32+64+128; 
    arrbig[20] <<=  8;
    arrbig[20] += 8+16;
    arrbig[20] <<=  8;
    arrbig[20] += 8+16;
    arrbig[20] <<=  8;
    arrbig[20] += 0; 
    ///////////
    arrbig[21] =  0; 
    arrbig[21] <<=  8;
    arrbig[21] += 0;
    arrbig[21] <<=  8;
    arrbig[21] += 8+16; 
    arrbig[21] <<=  8;
    arrbig[21] += 8+16;
    mt_gotoXY(12, 44);
    printf("12345678");
    big[0] = arrbig[20], big[1] = arrbig[21];
    bc_printbigchar(big, 13, 44, WHITE, YELLOW);
    
    arrbig[22] = 1+2 + 4 + 8 + 16 + 32+64+128; 
    arrbig[22] <<= 24;
    ///////////
    arrbig[23] =  0; 
    mt_gotoXY(12, 54);
    printf("12345678");
    big[0] = arrbig[22], big[1] = arrbig[23];
    bc_printbigchar(big, 13, 54, WHITE, YELLOW);
    
   
    

	//arr[0] = 1;
	mt_gotoXY(22,1);	
    printf("%d %d \n", arrbig[22], arrbig[23]);
    int file = open("bigchars.txt", O_RDWR | O_CREAT);
    bc_bigcharwrite (file, arrbig, 12);
    int file2 = open("bigchars.txt", O_RDONLY);
    int cunt = 0, arr2[2] = {0};
    bc_bigcharread(file2, arr2, 12, &cunt);
    printf("%d %d", arr2[22], arr2[23]);

    printf("\n");
    return 0;
}

