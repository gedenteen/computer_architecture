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
