#ifndef MY_SIGNALS
#define MY_SIGNALS

//#include <stdio.h>
#include <signal.h>
#include <sys/time.h>

int memx, memy;

void ms_signalhandler();
int ms_kbhit();
int ms_run();
int ms_converte_write(int value, char *sign, int *command, int *operand);
void ms_interface_static();
int ms_interface();
void ms_console_message(char st[]);
int ms_keyhandler();

#endif
