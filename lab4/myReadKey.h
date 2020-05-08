#ifndef MY_READKEY
#define MY_READKEY

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> //2 необходимые библиотеки для
#include <termios.h> //неканонического режима терминала

struct termios savetty; //объявление двух переменных для
struct termios tty; //хранения управляющих структур

struct termios current, backup;

enum keys
{
	KEY_F5,
    KEY_F6,
    KEY_L,
    KEY_S,
    KEY_R,
    KEY_T,
    KEY_I,
    KEY_UP,
    KEY_RIGHT,
    KEY_DOWN,
    KEY_LEFT,
    KEY_ENTER
};

int rk_readkey (enum keys *key);
int rk_mytermsave (void);
int rk_mytermrestore (void) ;
int rk_mytermregime (int regime, int vtime, int vmin, int echo, int sigint);

#endif
