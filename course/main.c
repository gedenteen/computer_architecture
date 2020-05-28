#include "../lab1/mySimpleComputer.h"
#include "../lab2/myTerm.h"
#include "../lab3/myBigChars.h"
#include "../lab4/myReadKey.h"
#include "../lab5/mySignals.h"
#include "myProcessor.h"

int main()
{
    ram[0] = 1379; //read [99]
    ram[1] = 1507; //write [99]
    ram[2] = 2659; //load [99]
    ram[3] = 2786; //store [98]
    ram[4] = 3939; //add [99]
    ram[5] = 4067; //sub [99]
    ram[6] = 4195; //divide [99]
    ram[7] = 4323; //mul [99]
    ram[8] = 5130; //jump [10]
    ram[10] = 5268; //jneg [20] //if accum < 0
    ram[11] = 4067; //sub [99] //else
    ram[12] = 5130; //jump [10] //цикл
    ram[20] = 5406; //jz [30] //if accum == 0
    ram[97] = 1;
    ram[21] = 3937; //add [97] //else
    ram[22] = 5140; //jump [20] //цикл
    ram[30] = 8419; //addc [99]
    ram[31] = 5504; //halt [0]
    ram[32] = 38901;


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

    SA_translator("file.sa", "filesa.o");
    ms_interface();



    return 0;
}


