#ifndef MY_BIGCHARS
#define MY_BIGCHARS

#include <stdio.h>
#include <fcntl.h>

int bc_printA(char *str);
int bc_box(int x1, int y1, int x2, int y2);
int bc_setbigcharpos (int * big, int x, int y, int value);
int bc_getbigcharpos(int * big, int x, int y, int *value);
int bc_printbigchar (int big[2], int x, int y, enum colors fclr, enum colors bclr);
int bc_bigcharwrite (int fd, int * big, int count);
int bc_bigcharread (int fd, int * big, int need_count, int * count);
int bc_showinterface();

#endif
