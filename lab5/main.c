#include "../lab1/mySimpleComputer.h"
#include "../lab2/myTerm.h"
#include "../lab3/myBigChars.h"
#include "../lab4/myReadKey.h"
#include "mySignals.h"

int _kbhit() {
    static bool inited = false;
    int left;

    if (!inited) {
        struct termios t;
        tcgetattr(0, &t);
        t.c_lflag &= ~ICANON;
        tcsetattr(0, TCSANOW, &t);
        setbuf(stdin, NULL);
        inited = true;
    }

    ioctl(0, FIONREAD, &left);

    return left;
}

int main()
{
    sc_memoryInit();
    sc_regInit();
    mt_clrscr();

    ram[0] = 10;
    ram[11] = 3;
    ram[22] = 28;
    ram[33] = 33789;
    ram[44] = 32768;
    ram[55] = -289;
    ram[66] = 7045;
    instructionCounter = 255;
    bool quit = false;

    ms_interface_static();
    while (quit == false)
    {
        ms_interface();
        enum keys key = -1;
        rk_readkey(&key);
        if (key == KEY_Q)
            quit = true;
        else
            ms_keyhandler(key);
    }
    return 0;
}


