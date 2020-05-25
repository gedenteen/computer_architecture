#include "../lab1/mySimpleComputer.h"
#include "../lab2/myTerm.h"
#include "../lab3/myBigChars.h"
#include "../lab4/myReadKey.h"
#include "../lab5/mySignals.h"
#include "myProcessor.h"

int ALU (int command, int operand)
{
    switch (command)
    {
        case 30: ///ADD
            accumulator += ram[operand];
            break;
        case 31: ///SUB
            accumulator -= ram[operand];
            break;
        case 32: ///DIVIDE
            if (ram[operand] == 0)
            {
                mt_gotoXY(24, 1);
                printf("division by zero");
                sc_regSet(DIVISON_BY_ZERO, 1);
                return -1;
            }
            accumulator /= ram[operand];
            break;
        case 33: ///MUL
            accumulator *= ram[operand];
            break;
        default:
            mt_gotoXY(24, 1);
            printf("Error in ALU()");
            return -1;
    }
    return 0;
}

int CU()
{
    int command, operand;
    int value = ram[instructionCounter];
    if (sc_commandDecode(value, &command, &operand) != 0)
    { ///получена не команда
        return -1;
    }
    if (command != 43 && (operand < 0 || operand > 99))
    { ///выход за границы памяти
        sc_regSet(GOING_BEYOND_MEMORY, 1);
        return -2;
    }
    mt_gotoXY(24, 1);
    printf("                                 ");
    switch (command)
    {
        case 10: ///READ
            mt_gotoXY(24, 1);
            mt_setbgcolor(RED);
            printf("Enter value in ram[%d] = ", operand);
            scanf("%d", &value);
            mt_setbgcolor(RESET);
            ram[operand] = value;
            //mt_gotoXY(24, 1);
            //printf("                                    ");
            break;
        case 11: ///WRITE
            mt_gotoXY(24, 1);
            mt_setbgcolor(RED);
            printf("Value in ram[%d] == %d", operand, ram[operand]);
            mt_setbgcolor(RESET);
            break;
        case 20: ///LOAD
            accumulator = ram[operand];
            break;
        case 21: ///STORE
            ram[operand] = accumulator;
            break;
        case 30:
        case 31:
        case 32:
        case 33:
            ALU(command, operand);
            break;
        case 40: ///JUMP
            instructionCounter = operand; //-1 из-за таймера
            memx = operand / 10;
            memy = operand % 10;
            return 2;
            break;
        case 41: ///JNEG
            if (accumulator < 0)
            {
                instructionCounter = operand;
                memx = operand / 10;
                memy = operand % 10;
                mt_gotoXY(25, 20);
                return 2;
            }
            break;
        case 42: ///JZ
            if (accumulator == 0)
            {
                instructionCounter = operand;
                memx = operand / 10;
                memy = operand % 10;
                return 2;
            }
            break;
        case 43: ///HALT
            return 1;
        default:
            mt_gotoXY(24, 1);
            printf("Error in CU()");
            break;
    }

    return 0;
}
