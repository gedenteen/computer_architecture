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
   	
   	
	printf("\E(0");
    printf("lrrrrrrrrrrrrrk\n");
    printf("x             x\n");
    printf("x             x\n");
    printf("x             x\n");
    printf("x             x\n");
    printf("x             x\n");
    printf("x             x\n");
    printf("mrrrrrrrrrrrrrj\n");
    printf("\E(B");
   	
   	
   	
    
    printf("\n");
    return 0;
}

