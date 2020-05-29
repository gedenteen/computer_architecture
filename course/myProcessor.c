#include "../lab1/mySimpleComputer.h"
#include "../lab2/myTerm.h"
#include "../lab3/myBigChars.h"
#include "../lab4/myReadKey.h"
#include "../lab5/mySignals.h"
#include "myProcessor.h"

int ALU (int command, int operand)
{
    int value;
    switch (command)
    {
        case 30: ///ADD
            value = accumulator + ram[operand];
            accumulator = value;
            break;
        case 31: ///SUB
            value = accumulator - ram[operand];
            accumulator = value;
            break;
        case 32: ///DIVIDE
            if (ram[operand] == 0)
            {
                ms_console_message("Division by zero");
                sc_regSet(DIVISON_BY_ZERO, 1);
                return -1;
            }
            accumulator /= ram[operand];
            break;
        case 33: ///MUL
            value = accumulator * ram[operand];
            accumulator = value;
            break;
        default:
            ms_console_message("Error in ALU()");
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
        sc_regSet(WRONG_COMMAND, 1);
        sc_regSet(IGNORING_CLOCK_PULSES, 1);
        return -1;
    }
    if (command != 43 && (operand < 0 || operand > 99))
    { ///выход за границы памяти
        sc_regSet(GOING_BEYOND_MEMORY, 1);
        return -2;
    }
    switch (command)
    {
        case 10: ///READ
            ms_console_message(" ");
            mt_gotoXY(24, 1);
            printf("Enter value in ram[%d] = ", operand);
            scanf("%d", &value);
            if (value >= 32768 || value < 0)
            {
                sc_regSet(OVERFLOW, 1);
                return -3;
            }
            ram[operand] = value;
            break;
        case 11: ///WRITE
            ms_console_message(" ");
            mt_gotoXY(24, 1);
            printf("Value in ram[%d] == %d", operand, ram[operand]);
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
            if (operand >= 100)
            {
                sc_regSet(GOING_BEYOND_MEMORY, 1);
                return -3;
            }
            instructionCounter = operand;
            memx = operand / 10;
            memy = operand % 10;
            return 2;
            break;
        case 41: ///JNEG
            if (accumulator < 0)
            {
                if (operand >= 100)
                {
                    sc_regSet(GOING_BEYOND_MEMORY, 1);
                    return -3;
                }
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
                if (operand >= 100)
                {
                    sc_regSet(GOING_BEYOND_MEMORY, 1);
                    return -3;
                }
                instructionCounter = operand;
                memx = operand / 10;
                memy = operand % 10;
                return 2;
            }
            break;
        case 43: ///HALT
            return 1;
        case 65: ///ADDC
            if (accumulator > 100 || accumulator < 0)
            {
                sc_regSet(GOING_BEYOND_MEMORY, 1);
                return -3;
            }
            if (operand >= 100)
            {
                sc_regSet(GOING_BEYOND_MEMORY, 1);
                return -3;
            }
            accumulator = ram[operand] + ram[accumulator];
            break;
        default:
            ms_console_message("Error in CU()");
            break;
    }

    return 0;
}

int charToInt(char ch)
{
    switch(ch)
    {
        case '0':
            return 0;
        case '1':
            return 1;
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5;
        case '6':
            return 6;
        case '7':
            return 7;
        case '8':
            return 8;
        case '9':
            return 9;
        case 'A':
            return 10;
        case 'B':
            return 11;
        case 'C':
            return 12;
        case 'D':
            return 13;
        case 'E':
            return 14;
        case 'F':
            return 15;
        default: ///не цифра
            return -1;
            break;
    }
}

int SA_translator(char *file_in, char *file_out)
{
    int cell, command, operand, ramt[100] = {0};
    char ch1, ch10[10];
    FILE *fp;
    if (!(fp = fopen(file_in, "rb"))) {
        fclose(fp);
        ms_console_message(" ");
        mt_gotoXY(24, 1);
        printf("Error: can't open file %s \n", file_in);
        return -1;
    }

    bool flag = true;
    int numst = 0;
    while (flag)
    {
        if (fread(&ch1, sizeof(char), 1, fp) == 0) ///если кол-во считанных символов == 0
            break;
        cell = charToInt(ch1) * 16;
        if (fread(&ch1, sizeof(char), 1, fp) == 0)
            goto error_;
        cell += charToInt(ch1);
        if (cell >= 100)
        {
            sc_regSet(GOING_BEYOND_MEMORY, 1);
            return 1;
        }

        if (fread(&ch1, sizeof(char), 1, fp) == 0) /// должен быть пробел
            goto error_;

        int u = 0;
        while (1)
        {
            if (fread(&ch1, sizeof(char), 1, fp) == 0)
                goto error_;
            if (ch1 == ' ')
                break;
            ch10[u] = ch1;
            u++;
            if (u == 10)
                goto error_;
        }
        if (strncmp(ch10, "READ", 4) == 0)
            command = 10;
        else if (strncmp(ch10, "WRITE", 5) == 0)
            command = 11;
        else if (strncmp(ch10, "LOAD", 4) == 0)
            command = 20;
        else if (strncmp(ch10, "STORE", 5) == 0)
            command = 21;
        else if (strncmp(ch10, "ADD", 3) == 0)
            command = 30;
        else if (strncmp(ch10, "SUB", 3) == 0)
            command = 31;
        else if (strncmp(ch10, "DIVIDE", 6) == 0)
            command = 32;
        else if (strncmp(ch10, "MUL", 3) == 0)
            command = 33;
        else if (strncmp(ch10, "JUMP", 4) == 0)
            command = 40;
        else if (strncmp(ch10, "JNEG", 4) == 0)
            command = 41;
        else if (strncmp(ch10, "JZ", 2) == 0)
            command = 42;
        else if (strncmp(ch10, "HALT", 4) == 0)
            command = 43;
        else if (strncmp(ch10, "ADDC", 4) == 0)
            command = 65;
        else if (strncmp(ch10, "=", 1) == 0)
        {
            if (fread(&ch1, sizeof(char), 1, fp) == 0)
                goto error_;
            if (ch1 == '-')
            {
                ramt[cell] = 16384;
                if (fread(&ch1, sizeof(char), 1, fp) == 0)
                    goto error_;
                ramt[cell] += charToInt(ch1) * 16 * 16 * 16;
                if (fread(&ch1, sizeof(char), 1, fp) == 0)
                    goto error_;
                ramt[cell] += charToInt(ch1) * 16 * 16;
                if (fread(&ch1, sizeof(char), 1, fp) == 0)
                    goto error_;
                ramt[cell] += charToInt(ch1) * 16;
                if (fread(&ch1, sizeof(char), 1, fp) == 0)
                    goto error_;
                ramt[cell] += charToInt(ch1);
                goto checkeof_;
            }
            else if (ch1 != '+')
                goto error_;

            int temp;
            if (fread(&ch1, sizeof(char), 1, fp) == 0)
                goto error_;
            temp = charToInt(ch1) * 16;
            if (fread(&ch1, sizeof(char), 1, fp) == 0)
                goto error_;
            temp += charToInt(ch1);
            ramt[cell] += temp << 7;

            if (fread(&ch1, sizeof(char), 1, fp) == 0)
                goto error_;
            ramt[cell] += charToInt(ch1) * 16;
            if (fread(&ch1, sizeof(char), 1, fp) == 0)
                goto error_;
            ramt[cell] += charToInt(ch1);

            goto checkeof_;
        }
        else ///если неизвестный номер команды
            goto error_;

        if (fread(&ch1, sizeof(char), 1, fp) == 0)
            goto error_;
        operand = charToInt(ch1) * 16;
        if (fread(&ch1, sizeof(char), 1, fp) == 0)
            goto error_;
        operand += charToInt(ch1);
        if (operand >= 128)
        {
            sc_regSet(OVERFLOW, 1);
            return -1;
        }

        command = command << 7;
        ramt[cell] = command + operand;

        checkeof_:
        while (1)
        {
            if (feof(fp))
            {
                flag = false;
                break;
            }
            if (fread(&ch1, sizeof(char), 1, fp) == 0)
            {
                flag = false;
                break;
            }
            if (ch1 == '\n')
                break;

        }
        numst++;
    }

    FILE *fq;
    if (!(fq = fopen(file_out, "wb")))
    {
        fclose(fp); fclose(fq);
        ms_console_message(" ");
        mt_gotoXY(24, 1);
        printf("Error: can't open file %s \n", file_out);
        return -1;
    }
    if (fwrite(ramt, sizeof(int), 100, fq) != 100) //возвращает число записанных элементов
    {
        fclose(fp); fclose(fq);
        ms_console_message(" ");
        mt_gotoXY(24, 1);
        printf("Error: failed to write information to %s \n", file_out);
        return 1;
    }
    fclose(fp); fclose(fq);
    ms_console_message(" ");
    mt_gotoXY(24, 1);
    printf("SA_translator return 0");
    return 0;

    error_:
    fclose(fp);
    ms_console_message(" ");
    mt_gotoXY(24, 1);
    printf("Error reading line %d in file %s", numst, file_in);
    return -1;
}

int SB_translator(char *file_in, char *file_out)
{
    FILE *f_in, *f_out;
    if (!(f_in = fopen(file_in, "rb")))
    {
        fclose(f_in);
        ms_console_message(" ");
        mt_gotoXY(24, 1);
        printf("Error: can't open file %s \n", file_in);
        return -1;
    }
    if (!(f_out = fopen(file_out, "wb")))
    {
        fclose(f_out);
        ms_console_message(" ");
        mt_gotoXY(24, 1);
        printf("Error: can't open file %s \n", file_out);
        return -1;
    }

    int line_num[100] = {0}, line = 0, SB_vars_max = 0;
    int SA_cell_command = 0, SA_cell_letter = 99;
    char ch1, ch10[10];
    struct SB_variable SB_vars[40];
    bool flag = true, read_error = false;
    while (flag)
    {
        //fscanf(f_in, "%s", ); ///если кол-во считанных символов == 0
        if (fread(&ch1, sizeof(char), 1, f_in) == 0) ///если кол-во считанных символов == 0
            break;
        line_num[line] = charToInt(ch1) * 10;
        if (fread(&ch1, sizeof(char), 1, f_in) == 0)
            goto error_;
        line_num[line] += charToInt(ch1);

        if (fread(&ch1, sizeof(char), 1, f_in) == 0)
            goto error_;
        if (ch1 != ' ')
            goto error_;

        int u = 0;
        while (1)
        {
            if (fread(&ch1, sizeof(char), 1, f_in) == 0)
                goto error_;
            if (ch1 == '\n')
            {
                ms_console_message("error1");
                goto error_;
            }

            if (ch1 == ' ')
                break;
            ch10[u] = ch1; ///иначе
            u++;
        }

        if (strncmp(ch10, "REM", 3) == 0)
        {
            while (1)
            {
                fread(&ch1, sizeof(char), 1, f_in);
                if (ch1 == '\n')
                    break;
            }
        }
        else if (strncmp(ch10, "INPUT", 5) == 0)
        {
            if (fread(&ch1, sizeof(char), 1, f_in) == 0)
                goto error_;
//            for (u = 0; u < SB_vars_max; U++)
//            {
//                if SB_vars[u].letter
//            }
            SB_vars[SB_vars_max].letter = ch1; ///новая переменная, ее буква
            SB_vars[SB_vars_max].cell = SA_cell_letter; ///ее ячейка в ram
            fprintf(f_out, "%02X READ %02X \n", SA_cell_command, SA_cell_letter); ///записать это в file_out
            SB_vars_max++; ///кол-во переменных +1
            SA_cell_command++; ///номер следующей команды в file_out +1
            SA_cell_letter--; ///номер для следующей переменной сдвигается назад

            if (fread(&ch1, sizeof(char), 1, f_in) == 0)
                goto error_;
            if (ch1 != '\n')
                goto error_;
        }
        else if (strncmp(ch10, "OUTPUT", 6) == 0)
        {
//            if (fread(&ch1, sizeof(char), 1, f_in) == 0)
//                goto error_;
//            for (u = 0; u < SB_vars_max; U++)
//            {
//                if SB_vars[u].letter
//            }
//            SB_vars[SB_vars_max].letter = ch1; ///новая переменная, ее буква
//            SB_vars[SB_vars_max].cell = SA_cell_letter; ///ее ячейка в ram
//            fprintf(f_out, "%02X READ %02X \n", SA_cell_command, SA_cell_letter); ///записать это в file_out
//            SB_vars_max++; ///кол-во переменных +1
//            SA_cell_command++; ///номер следующей команды в file_out +1
//            SA_cell_letter--; ///номер для следующей переменной сдвигается назад
//
//            if (fread(&ch1, sizeof(char), 1, f_in) == 0)
//                goto error_;
//            if (ch1 != '\n')
//                goto error_;
        }


        line++;
    }

    ms_console_message("ok");
    fclose(f_in); fclose(f_out);
    return 0;


    error_:
    //ms_console_message(ch10);
    ms_console_message(" ");
    mt_gotoXY(24, 1);
    printf("Error in line %d in file %s", line, file_in);
    fclose(f_in); fclose(f_out);
    return -1;

}

