#ifndef MY_PROCESSOR
#define MY_PROCESSOR

struct SB_variable
{
    char letter;
    int cell;
};

int ALU (int command, int operand);
int CU();
int charToInt(char ch);
int SA_translator(char *file_in, char *file_out);
int SB_translator(char *file_in, char *file_out);

#endif
