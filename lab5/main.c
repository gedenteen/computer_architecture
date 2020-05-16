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

    bool quit = false;
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


