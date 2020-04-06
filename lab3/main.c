#include "../lab1/mySimpleComputer.h"
#include "../lab2/myTerm.h"
#include "myBigChars.h"

int main()
{
	mt_clrscr();
	//проверка работы функций
	bc_box(1, 1, 21, 75);
	int file2 = open("bigchars.txt", O_RDONLY);
	int cnt = 0, arrbig[24] = {0};
	bc_bigcharread(file2, arrbig, 12, &cnt); //считы
	int big[2] = {0}, i, x1 = 2, y1 = 2;
	for (i = 0; i < 24; i += 2)
	{
	if (i == 14)
		x1 += 10, y1 = 2; 
	mt_gotoXY(x1, y1);
	printf("12345678");
	big[0] = arrbig[i], big[1] = arrbig[i+1];
	bc_printbigchar(big, x1+1, y1, RED, GRAY);

	y1 += 10;
	}
	bc_setbigcharpos (big, 2, 2, 1);
	bc_setbigcharpos (big, 3, 2, 0);
	bc_printbigchar(big, x1+1, y1, GREEN, RESET);
	/*printf("%d %d \n", arrbig[0], arrbig[1]);
	int file = open("bigchars.txt", O_RDWR | O_CREAT);
	bc_bigcharwrite (file, arrbig, 12);*/

	mt_clrscr();	
	bc_showinterface();
	

	mt_gotoXY(24,1);	
	return 0;
}

