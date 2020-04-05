#include "../lab1/mySimpleComputer.h"
#include "../lab2/myTerm.h"
#include "myBigChars.h"

int main()
{
    //команда в терминале: man console_codes
    int file2 = open("bigchars.txt", O_RDONLY);
    int cnt = 0, arrbig[24] = {0};
    bc_bigcharread(file2, arrbig, 12, &cnt);
    
    mt_clrscr();
    bc_box(1, 3, 21, 70);
    printf("cnt == %d, arrbig[22] == %d, [23] == %d\n", cnt, arrbig[22], arrbig[23]);

    
    int big[2] = {0}, i, x1 = 2, y1 = 4;
    x1 = 2; y1 = 4;
    for (i = 0; i < 24; i += 2)
    {
    	if (i == 12)
    		x1 += 10, y1 = 4; 
    		
    	mt_gotoXY(x1, y1);
    	printf("i==%d", i);
    	big[0] = arrbig[i], big[1] = arrbig[i+1];
    	bc_printbigchar(big, x1+1, y1, BLUE, RESET);
    	
    	y1 += 10;
    }
    
    
   
    bc_setbigcharpos(big, 2, 2, 3);
    bc_box(0, 2, 4, 4);
    bc_getbigcharpos(big, -1, 3, &i);

	mt_gotoXY(27,1);	
    /*printf("%d %d \n", arrbig[0], arrbig[1]);
    int file = open("bigchars.txt", O_RDWR | O_CREAT);
    bc_bigcharwrite (file, arrbig, 12);*/
    
    
    printf("\n");
    return 0;
}

