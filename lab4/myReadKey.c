#include "../lab1/mySimpleComputer.h"
#include "../lab2/myTerm.h"
#include "../lab3/myBigChars.h"
#include "myReadKey.h"

int rk_readkey (enum keys *key)
{
    if ( !isatty(0) ) { /*Проверка: стандартный ввод - терминал?*/
      fprintf (stderr, "stdin not terminal\n");
      exit (1); /* Ст. ввод был перенаправлен на файл, канал и т.п. */
    };

    tcgetattr (0, &tty); //функция получения установки терминала
    savetty = tty; /* Сохранить упр. информацию канонического режима */
    tty.c_lflag &= ~(ICANON|ECHO|ISIG);
    tty.c_cc[VMIN] = 1;
    tcsetattr (0, TCSAFLUSH, &tty); //Перевод драйвера клавиатуры в неканонический режим ввода

    char buf[10] = "\0";
    read(STDIN_FILENO, buf, 10);

    if (!strcmp(buf, "\E[15~"))
        *key = KEY_F5;
    else if (!strcmp(buf, "\E[17~"))
        *key = KEY_F6;
    else if (!strcmp(buf, "l"))
        *key = KEY_L;
    else if (!strcmp(buf, "s"))
        *key = KEY_S;
    else if (!strcmp(buf, "r"))
        *key = KEY_R;
    else if (!strcmp(buf, "t"))
        *key = KEY_T;
    else if (!strcmp(buf, "i"))
        *key = KEY_I;
    else if (!strcmp(buf, "\E[A"))
        *key = KEY_UP;
    else if (!strcmp(buf, "\E[C"))
        *key = KEY_RIGHT;
    else if (!strcmp(buf, "\E[B"))
        *key = KEY_DOWN;
    else if (!strcmp(buf, "\E[D"))
        *key = KEY_LEFT;
    else if (!strcmp(buf, "\n"))
        *key = KEY_ENTER;

    tcsetattr (0, TCSAFLUSH, &savetty); //Восстановление канонического режима ввода
    return 0;
}

int rk_mytermsave()
{
    return tcsetattr(STDOUT_FILENO, TCSANOW, &tty);
}

int rk_mytermrestore()
{
    return tcsetattr(STDOUT_FILENO, TCSANOW, &savetty);
}

int rk_mytermregime(int regime, int vtime, int vmin, int echo, int sigint)
{
    if (tcgetattr(STDOUT_FILENO, &tty))
        return 1;

    if (regime)
        tty.c_lflag |= ICANON;
    else tty.c_lflag &= ~ICANON;

    if (echo)
        tty.c_lflag |= ECHO;
    else tty.c_lflag &= ~ECHO;

    if (sigint)
        tty.c_lflag |= ISIG;
    else tty.c_lflag &= ~ISIG;

    tty.c_cc[VTIME] = vtime;
    tty.c_cc[VMIN] = vmin;

    return tcsetattr(STDOUT_FILENO, TCSANOW, &tty);
}
int rk_interface()
{
	int i, x1 = 1, y1 = 2;
	bc_box(1, 1, 12, 61); //память
	mt_setfgcolor(YELLOW);
	mt_gotoXY(1, 25);
	printf(" Memory: ");
	mt_setbgcolor(RESET);
	//mt_setfgcolor(WHITE);
	for (i = 0; i < 100; i++) {
		if (i % 10 == 0)
			x1++, y1 = 2;
		mt_gotoXY(x1, y1);
		if (ram[i] < 0)
		{
            mt_setbgcolor(BLUE);
            printf("%05d", ram[i] % 10000);
		}
        else
        {
            if (ram[i] > 0)
                mt_setbgcolor(RED);
            printf("+");
            printf("%04d", ram[i] % 10000);
        }
		mt_setbgcolor(RESET);
		y1 += 6;
	}
	mt_setbgcolor(RESET); //рамки
	bc_box(1, 62, 3, 85);
	bc_box(4, 62, 6, 85);
	bc_box(7, 62, 9, 85);
	bc_box(10, 62, 12, 85);
	bc_box(13, 1, 22, 46);
	bc_box(13, 47, 22, 85);
	mt_setfgcolor(YELLOW); //названия
	mt_gotoXY(1, 67);
	printf(" accumulator ");
	mt_gotoXY(4, 64);
	printf(" instructionCounter ");
	mt_gotoXY(7, 69);
	printf(" Operation ");
	mt_gotoXY(10, 71);
	printf(" Flags ");
	mt_gotoXY(13, 49);
	printf(" Keys ");
	mt_setfgcolor(WHITE); //содержимое
	mt_gotoXY(2, 71);
	if (accumulator >= 0)
		printf("+");
	printf("%4.4d", accumulator);
	mt_gotoXY(5, 71);
	if (instructionCounter >= 0)
		printf("+");
	printf("%4.4d", instructionCounter);
	mt_gotoXY(8, 70);
	printf("+00 : 00");
	mt_gotoXY(11, 70);
	printf(" O E V M ");

	int file2 = open("../lab3/bigchars.txt", O_RDONLY); //бигчары
	int cnt = 0, arrbig[24] = {0}, big[2] = {0};
	bc_bigcharread(file2, arrbig, 12, &cnt);
	if (instructionCounter >= 0)
		big[0] = arrbig[20], big[1] = arrbig[21];
	else
		big[0] = arrbig[22], big[1] = arrbig[23];
	bc_printbigchar(big, 14, 2, WHITE, RESET);
	int temp = instructionCounter % 10000 / 1000;
	big[0] = arrbig[temp*2], big[1] = arrbig[temp*2+1];
	bc_printbigchar(big, 14, 11, WHITE, RESET);
	temp = instructionCounter % 1000 / 100;
	big[0] = arrbig[temp*2], big[1] = arrbig[temp*2+1];
	bc_printbigchar(big, 14, 20, WHITE, RESET);
	temp = instructionCounter % 100 / 10;
	big[0] = arrbig[temp*2], big[1] = arrbig[temp*2+1];
	bc_printbigchar(big, 14, 29, WHITE, RESET);
	temp = instructionCounter % 10;
	big[0] = arrbig[temp*2], big[1] = arrbig[temp*2+1];
	bc_printbigchar(big, 14, 38, WHITE, RESET);

	mt_gotoXY(14, 48);
	printf("l - load");
	mt_gotoXY(15, 48);
	printf("s - save");
	mt_gotoXY(16, 48);
	printf("r - run");
	mt_gotoXY(17, 48);
	printf("t - step");
	mt_gotoXY(18, 48);
	printf("i - reset");
	mt_gotoXY(19, 48);
	printf("F5 - accumulator");
	mt_gotoXY(20, 48);
	printf("F6 - instructionCounter");
	mt_gotoXY(23, 1);

	return 0;
}

