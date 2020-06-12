#ifndef MY_SIMPLE_COMPUTER_H
#define MY_SIMPLE_COMPUTER_H

#include <stdio.h>

#define OVERFLOW 1
#define DIVISON_BY_ZERO 2
#define GOING_BEYOND_MEMORY 3
#define IGNORING_CLOCK_PULSES 4
#define WRONG_COMMAND 5

int ram[100], registr;
short int accumulator, instructionCounter;

int sc_memoryInit();
int sc_memorySet(int address, int value);
int sc_memoryGet(int address, int *value);
int sc_memoryShow();
int sc_memorySave(char *filename);
int sc_memoryLoad(char *filename);
int sc_regInit();
int sc_regSet(int reg, int value);
int sc_regGet (int reg, int * value);
int sc_commandEncode (int command, int operand, int * value);
int sc_commandDecode (int value, int * command, int * operand);


#endif
