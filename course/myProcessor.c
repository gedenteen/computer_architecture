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

FILE *f_in, *f_out;
int line_num[100] = {0}, cmd_count = 0, SB_vars_max = 0;
int SA_cell_command = 0, SA_cell_letter = 99;
char ch1, ch10[10];
struct SB_variable SB_vars[40];

int SB_commands()
{
    int u;
    if (strncmp(ch10, "REM", 3) == 0)
    {
        cmd_count--;
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
            return -1;
        SB_vars[SB_vars_max].letter = ch1; ///новая переменная, ее буква
        SB_vars[SB_vars_max].cell = SA_cell_letter; ///ее ячейка в ram
        fprintf(f_out, "%02X READ %02X \n", SA_cell_command, SA_cell_letter); ///записать это в file_out
        SB_vars_max++; ///кол-во переменных +1
        SA_cell_command++; ///номер следующей команды в file_out +1
        SA_cell_letter--; ///номер для следующей переменной сдвигается назад

        if (fread(&ch1, sizeof(char), 1, f_in) == 0)
            return -1;
        if (ch1 != '\n')
            return -1;
    }
    else if (strncmp(ch10, "OUTPUT", 6) == 0)
    {
        if (fread(&ch1, sizeof(char), 1, f_in) == 0)
            return -1;
        bool find_ = false;
        for (u = 0; u < SB_vars_max; u++)
        {
            if (SB_vars[u].letter == ch1)
            {
                find_ = true;
                break;
            }

        }
        if (!find_)
            return -1;

        //SB_vars[SB_vars_max].letter = ch1; ///новая переменная, ее буква
        //SB_vars[SB_vars_max].cell = SA_cell_letter; ///ее ячейка в ram
        fprintf(f_out, "%02X WRITE %02X \n", SA_cell_command, SB_vars[u].cell); ///записать это в file_out
        SA_cell_command++; ///номер следующей команды в file_out +1

        if (fread(&ch1, sizeof(char), 1, f_in) == 0)
            return -1;

        if (ch1 != '\n')
            return -1;
    }
    else if (strncmp(ch10, "GOTO", 4) == 0)
    {
        if (fread(&ch1, sizeof(char), 1, f_in) == 0) ///если кол-во считанных символов == 0
            return -1;
        int temp = charToInt(ch1) * 10;
        if (fread(&ch1, sizeof(char), 1, f_in) == 0)
            return -1;
        temp += charToInt(ch1);

        bool find_ = false;
        for (u = 0; u <= cmd_count; u++)
        {
            if (line_num[u] == temp)
            {
                fprintf(f_out, "%02X JUMP %02X \n", SA_cell_command, u);
                SA_cell_command++;
                find_ = true;
                break;
            }
        }
        if (find_ == false)
            return -1;
        mt_gotoXY(24,40); printf("u=%d", u);

        if (fread(&ch1, sizeof(char), 1, f_in) == 1)
            if (ch1 != '\n')
                return -1;
    }
    else if (strncmp(ch10, "IF", 2) == 0)
    {
        char value1, value2, sign;
        int u1 = -1, u2 = -1;
        int ivalue1 = -1, ivalue2 = -1;

        fread(&value1, sizeof(char), 1, f_in);
        if (charToInt(value1) >= 0 && charToInt(value1) <= 9) ///если прочитано число
        {
            ivalue1 = charToInt(value1);
            while (1)
            {
                fread(&ch1, sizeof(char), 1, f_in);
                if (ch1 == ' ')
                    break;
                else
                    ivalue1 = ivalue1 * 10 + charToInt(ch1);
            }
        }
        else ///иначе, если прочитана буква-переменная
            fread(&ch1, sizeof(char), 1, f_in); ///прочитать пробел
        //mt_gotoXY(23, 35); printf("qwe10 done, %d", ivalue1);

        fread(&sign, sizeof(char), 1, f_in);
        //mt_gotoXY(23, 35); printf("qwe11 done, %c", sign);

        fread(&ch1, sizeof(char), 1, f_in);
        fread(&value2, sizeof(char), 1, f_in);
        if (charToInt(value2) >= 0 && charToInt(value2) <= 9) ///если прочитано число
        {
            ivalue2 = charToInt(value2);
            while (1)
            {
                fread(&ch1, sizeof(char), 1, f_in);
                if (ch1 == ' ')
                    break;
                else
                    ivalue2 = ivalue2 * 10 + charToInt(ch1);
            }
        } ///прочитано условие
        //mt_gotoXY(23, 35); printf("qwe12 done, %d %c", ivalue2, value2);

        for (u = 0; u <= SB_vars_max; u++)
        { ///поиск указанной буквы-переменной(ых)
            if (ivalue1 == -1 && SB_vars[u].letter == value1)
            {
                u1 = u;
            }
            if (ivalue2 == -1 && SB_vars[u].letter == value2)
            {
                u2 = u;
            }
        }

        if (sign == '>' || sign == '=')
        {
            if (ivalue1 == -1)
            {
                fprintf(f_out, "%02X LOAD %02X \n", SA_cell_command, SB_vars[u1].cell);
                SA_cell_command++;
            }
            else
            {
                fprintf(f_out, "%02X = +%04X \n", SA_cell_letter, ivalue1);
                fprintf(f_out, "%02X LOAD %02X \n", SA_cell_command, SA_cell_letter);
                SA_cell_letter--; ///номер для следующей переменной сдвигается назад
                SA_cell_command++;
            }
            if (ivalue2 == -1)
            {
                fprintf(f_out, "%02X SUB %02X \n", SA_cell_command, SB_vars[u2].cell);
                SA_cell_command++;
            }
            else
            {
                fprintf(f_out, "%02X = +%04X \n", SA_cell_letter, ivalue2);
                fprintf(f_out, "%02X SUB %02X \n", SA_cell_command, SA_cell_letter);
                SA_cell_letter--; ///номер для следующей переменной сдвигается назад
                SA_cell_command++;
            }

        }
        else if (sign == '<')
        {

            if (ivalue2 == -1)
            {
                fprintf(f_out, "%02X LOAD %02X \n", SA_cell_command, SB_vars[u2].cell);
                SA_cell_command++;
            }
            else
            {
                fprintf(f_out, "%02X = +%04X \n", SA_cell_letter, ivalue2);
                fprintf(f_out, "%02X LOAD %02X \n", SA_cell_command, SA_cell_letter);
                SA_cell_letter--; ///номер для следующей переменной сдвигается назад
                SA_cell_command++;
            }
            if (ivalue1 == -1)
            {
                fprintf(f_out, "%02X SUB %02X \n", SA_cell_command, SB_vars[u1].cell);
                SA_cell_command++;
            }
            else
            {
                fprintf(f_out, "%02X = +%04X \n", SA_cell_letter, ivalue1);
                fprintf(f_out, "%02X SUB %02X \n", SA_cell_command, SA_cell_letter);
                SA_cell_letter--; ///номер для следующей переменной сдвигается назад
                SA_cell_command++;
            }
        }
        else
            return -1;
        //mt_gotoXY(23, 35); printf("qwe30 done");

        int temp_cell = SA_cell_command; ///сохранение ячейки для JNEG ИЛИ JZ
        SA_cell_command++;
        //char ch1_temp = ch1;

        if (ivalue2 == -1)
            fread(&ch1, sizeof(char), 1, f_in); ///прочитать пробел
        //mt_gotoXY(23, 58); printf("iv2=%d ch1=%c", ivalue2, ch1);
        u = 0;
        while (1) ///чтение команды после IF
        {
            if (fread(&ch1, sizeof(char), 1, f_in) == 0)
                return -1;
            if (ch1 == '\n')
            {
                ms_console_message("error1");
                return -1;
            }

            if (ch1 == ' ')
            {
                break;
            }

            ch10[u] = ch1; ///иначе
            u++;
        }


        SB_commands();
        //mt_gotoXY(23, 35); printf("qwe50 done");

        if (sign == '>' || sign == '<')
            fprintf(f_out, "%02X JNEG %02X \n", temp_cell, SA_cell_command);
        else /// '='
            fprintf(f_out, "%02X JZ %02X \n", temp_cell, SA_cell_command);
        //SA_cell_command++;

//        if (fread(&ch1, sizeof(char), 1, f_in) == 1)
//            if (ch1 != '\n' && ch1 != '\0')
//                return -1;
        //mt_gotoXY(23, 35); printf("qwe60 done");
    }
    else if (strncmp(ch10, "LET", 3) == 0) ///-- IF --///
    {
        int temp = 0, number, i, index;
        char l[100], sign;
        bool first_time = true;

        fread(&l[0], sizeof(char), 1, f_in); /// 'C'
        fread(&ch1, sizeof(char), 1, f_in); /// ' '
        fread(&ch1, sizeof(char), 1, f_in); /// '='
        fread(&ch1, sizeof(char), 1, f_in); /// ' '
        for (i = 1; i < 100; i++)
        {
            if (fread(&l[i], sizeof(char), 1, f_in) == 0)
                break;
            if (l[i] == '\n')
                break;

            number = charToInt(l[i]);
            if (number >= 0 && number <= 9)
            { ///если задано число
                temp = number;
                while (1)
                {
                    if (fread(&l[i], sizeof(char), 1, f_in) == 0)
                        break;
                    if (l[i] == '\n' || l[i] == ' ')
                    {
                        ungetc(l[i], f_in); ///вернуть указатель в файле на 1
                        break;
                    }

                    number = charToInt(l[i]);
                    if (number >= 0 && number <= 9)
                        temp = temp * 10 + number;
                } ///конец считывания числа
                fprintf(f_out, "%02X = +%04X \n", SA_cell_letter, temp);
                if (first_time)
                {
                    first_time = false;
                    fprintf(f_out, "%02X LOAD %02X \n", SA_cell_command, SA_cell_letter);
                    SA_cell_command++;
                }
                else switch (sign)
                    {
                        case '+':
                            fprintf(f_out, "%02X ADD %02X \n", SA_cell_command, SA_cell_letter);
                            SA_cell_command++;
                            break;
                        case '-':
                            fprintf(f_out, "%02X SUB %02X \n", SA_cell_command, SA_cell_letter);
                            SA_cell_command++;
                            break;
                        case '/':
                            fprintf(f_out, "%02X DIVIDE %02X \n", SA_cell_command, SA_cell_letter);
                            SA_cell_command++;
                            break;
                        case '*':
                            fprintf(f_out, "%02X MUL %02X \n", SA_cell_command, SA_cell_letter);
                            SA_cell_command++;
                            break;
                        default:
                            return -1;
                    }
                //index = SA_cell_letter;
                SA_cell_letter--;
            }
            else for (u = 0; u <= SB_vars_max; u++) ///если перменная
            {
                if (SB_vars[u].letter == l[i])
                {
                    if (first_time)
                    {
                        first_time = false;
                        fprintf(f_out, "%02X LOAD %02X \n", SA_cell_command, SB_vars[u].cell);
                        SA_cell_command++;
                    }
                    else switch (sign)
                    {
                        case '+':
                            fprintf(f_out, "%02X ADD %02X \n", SA_cell_command, SB_vars[u].cell);
                            SA_cell_command++;
                            break;
                        case '-':
                            fprintf(f_out, "%02X SUB %02X \n", SA_cell_command, SB_vars[u].cell);
                            SA_cell_command++;
                            break;
                        case '/':
                            fprintf(f_out, "%02X DIVIDE %02X \n", SA_cell_command, SB_vars[u].cell);
                            SA_cell_command++;
                            break;
                        case '*':
                            fprintf(f_out, "%02X MUL %02X \n", SA_cell_command, SB_vars[u].cell);
                            SA_cell_command++;
                            break;
                        default:
                            return -1;
                    }
                    break;
                }
            }
            if (first_time == false) ///если сложение, умножение
                switch (l[i])
                {
                    case '+':
                    case '-':
                    case '/':
                    case '*':
                        sign = l[i];
                        break;
                    default:
                        break;
                }

            fread(&ch1, sizeof(char), 1, f_in); /// ' '
            if (ch1 == '\n')
                break;
        }


        int u0 = -1;
        for (u = 0; u <= SB_vars_max; u++)
        {
            if (SB_vars[u].letter == l[0])
                u0 = u;
        }
        if (u0 == -1) ///если не объявлена переменная, которая перед =
        {
            SB_vars[SB_vars_max].letter = l[0]; ///новая переменная, ее буква
            SB_vars[SB_vars_max].cell = SA_cell_letter; ///ее ячейка в ram
            u0 = SB_vars_max;
            SB_vars_max++; ///кол-во переменных +1
            SA_cell_letter--; ///номер для следующей переменной сдвигается назад
        }
        fprintf(f_out, "%02X STORE %02X \n", SA_cell_command, SB_vars[u0].cell);
        SA_cell_command++;
    }
    else if (strncmp(ch10, "END", 3) == 0)
    {
        fprintf(f_out, "%02X HALT 00 \n", SA_cell_command);
        SA_cell_command++;
    }
    else
        return -1;

    //error_:
    return 0;
}
int SB_translator(char *file_in, char *file_out)
{
    for (int i = 0; i < 100; i++)
    {
        line_num[i] = 0;
    }
    cmd_count = 0;
    for (int i = 0; i <= SB_vars_max; i++)
    {
        SB_vars[i].letter = 0;
        SB_vars[i].cell = 0;
    }
    SB_vars_max = 0;
    SA_cell_command = 0, SA_cell_letter = 99; ///конец обнуления

    int real_line_num = 0;
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

    bool flag = true;
    while (flag)
    {
        real_line_num++;
        if (fread(&ch1, sizeof(char), 1, f_in) == 0) ///если кол-во считанных символов == 0
            break;
        line_num[cmd_count] = charToInt(ch1) * 10;
        if (fread(&ch1, sizeof(char), 1, f_in) == 0)
            goto error_;
        line_num[cmd_count] += charToInt(ch1);

        if (fread(&ch1, sizeof(char), 1, f_in) == 0)
            goto error_;
        //mt_gotoXY(23, 15); printf("%d", line_num[cmd_count]);
        if (ch1 != ' ')
            goto error_;

        int u = 0;
        while (1)
        {
            if (fread(&ch1, sizeof(char), 1, f_in) == 0)
                break;
            if (ch1 == '\n')
                break;
            if (ch1 == ' ')
                break;
            ch10[u] = ch1; ///иначе
            u++;
        }

        if (SB_commands(f_in, f_out) == -1)
        {
            goto error_;
        }

        cmd_count++;
    }

    ms_console_message("SB_translator return 0");
    fclose(f_in); fclose(f_out);
    return 0;


    error_:
    ms_console_message("");
    printf("Error in line %d in file %s", real_line_num, file_in);
    fclose(f_in); fclose(f_out);
    return -1;

}

