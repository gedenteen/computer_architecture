#include "../lab1/mySimpleComputer.h"
#include "../lab2/myTerm.h"
#include "../lab3/myBigChars.h"
#include "myReadKey.h"

int main()
{
    sc_memoryInit();
    sc_regInit();
    bool quit = false;
    accumulator = 0, instructionCounter = 0;
    while (!quit)
    {
        mt_clrscr();
        rk_interface();
        printf("Commands: \n");
        printf("0. Quit: \n");
        printf("1. Change the values in memory (address, value) \n");
        printf("2. Set flag case (reg, value) \n");
        printf("3. Use \"keys\" \n");
        int command;
        scanf("%d", &command);
        if (command == 0)
            break;
        else if (command == 1)
        {
            instructionCounter++;
            int address, value;
            scanf("%d %d", &address, &value);
            sc_memorySet(address, value);
        }
        else if (command == 2)
        {
            instructionCounter++;
            int reg, value;
            scanf("%d %d", &reg, &value);
            sc_regSet (reg, value);
        }
        else if (command == 3)
        {
            enum keys key = -1;
            rk_readkey(&key);
            if (key == KEY_L)
                sc_memoryLoad("file.dat");
            if (key == KEY_S)
                sc_memorySave("file.dat");
        }

    }




	return 0;
}


