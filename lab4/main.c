#include "../lab1/mySimpleComputer.h"
#include "../lab2/myTerm.h"
#include "../lab3/myBigChars.h"
#include "myReadKey.h"

int main()
{
	//mt_clrscr();

    //printf("\E(0");
//   	printf("1q 2w 3e 4r 6t 7y 8u 9i 10o 11p [ ] 2 \n");
//
//   	printf("1a 2s 3d 4f 5g 6h 7j 8k 9l ; ' \n");
//   	printf("lqqqqqqqk\n");
//   	printf("lrrrrrrrrrrrrrk\n");
//   	printf("looooooooook\n");
//   	printf("lpppppppk\n");
//   	printf("1z 2x 3c 4v 5b 6n 7m 8, 9<\n\n");
//
//
//
//    char qwer[10];
//    scanf("%c", &qwer);

//    printf("\E(0");
//    char qwer[10];
//    scanf("%c %c", &qwer, &qwer);
//    printf("\E(B");
    mt_clrscr();
    enum keys key = 0;
    rk_readkey(&key);
    printf(" %d \n", key);


	return 0;
}


