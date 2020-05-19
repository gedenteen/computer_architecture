#include "../lab1/mySimpleComputer.h"
#include "../lab2/myTerm.h"
#include "../lab3/myBigChars.h"
#include "../lab4/myReadKey.h"
#include "../lab5/mySignals.h"
#include "myProcessor.h"

int main()
{
    ram[1] = 1290; //read [10]
    ram[2] = 1410; //write [2]
    ram[3] = 2562; //load [2]
    ram[4] = 2702; //store [14]
    ram[5] = 3854; //add [14]
    ram[6] = 3982; //sub[14]
    ram[7] = 4238; //mul [14]
    ram[8] = 4196; //sub [100]

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


