#include "../lab1/mySimpleComputer.h"
#include "../lab2/myTerm.h"
#include "../lab3/myBigChars.h"
#include "../lab4/myReadKey.h"
#include "../lab5/mySignals.h"
#include "myProcessor.h"

int main()
{
    ram[1] = 1290;
    instructionCounter = 0;

    sc_regInit();
    mt_clrscr();

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


