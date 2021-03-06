#include "../lab1/mySimpleComputer.h"
#include "../lab2/myTerm.h"
#include "../lab3/myBigChars.h"
#include "../lab4/myReadKey.h"
#include "mySignals.h"

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
    instructionCounter = 0;
    sc_regSet(OVERFLOW, 1);
    sc_regSet(DIVISON_BY_ZERO, 1);
    sc_regSet(GOING_BEYOND_MEMORY, 1);
    //sc_regSet(IGNORING_CLOCK_PULSES, 1);
    sc_regSet(WRONG_COMMAND, 1);
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


