#ifndef MY_SIGNALS
#define MY_SIGNALS

//#include <stdio.h>
#include <signal.h>
#include <sys/time.h>

int memx, memy;

int ms_signalhandler();
int ms_run();
int ms_interface();
int ms_keyhandler();

#endif
