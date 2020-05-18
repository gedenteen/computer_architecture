#include "../lab1/mySimpleComputer.h"
#include "../lab2/myTerm.h"
#include "../lab3/myBigChars.h"
#include "../lab4/myReadKey.h"
#include "../lab5/mySignals.h"
#include "myProcessor.h"

int CU()
{
    int command, operand;
    int value = ram[instructionCounter];
    if (sc_commandDecode(value, &command, &operand) != 0)
    {
        mt_gotoXY(25, 3);
        printf("%d %d %d", value, command, operand);
        return 1;
    }
    switch (command)
    {
        case 10:
            mt_gotoXY(25, 3);
            printf("Enter value in ram[%d] = ", operand);
            scanf("%d", &value);
            ram[operand] = value;
            break;
        default:
            mt_gotoXY(25, 3);
            printf("Error in CU()");
    }

    return 0;
}
