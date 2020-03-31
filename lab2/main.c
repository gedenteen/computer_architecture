#include "myTerm.h"
#include "../lab1/mySimpleComputer.h"

int main() {

	mt_clrscr();
	int rows, cols;
	mt_getscreensize (&rows, &cols);
	printf ("lines %d\n", rows);
	printf ("columns %d\n", cols);
	

	//printf("its me %8X \n", -2);
	sc_memorySet(75, 75);
	sc_memorySet(68, -29);
	sc_memorySet(42, 32);
	mt_showinterface ();


	return 0;
}
