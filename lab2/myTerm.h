#ifndef MY_TERM
#define MY_TERM

#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

enum colors 
{ 
	GRAY, //=0
	RED, //=1
	GREEN, 
	YELLOW,
	BLUE,
	MAGENTA,
	CYAN = 6,
	WHITE, //7
	RESET
};

int mt_clrscr (void);
int mt_gotoXY (int, int);
int mt_getscreensize (int * rows, int * cols);
int mt_setfgcolor (enum colors color);
int mt_setbgcolor (enum colors color);
int mt_showinterface (void);

#endif
