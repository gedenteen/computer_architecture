#include "mySimpleComputer.h"

// extern ram[], registr; //указывает, что эти переменные объявлены в другом
// файле extern OVERFLOW, DIVISON_BY_ZERO, GOING_BEYOND_MEMORY,
// IGNORING_CLOCK_PULSES, WRONG_COMMAND;

int sc_memoryInit() {
  for (int i = 0; i < 100; i++) {
    ram[i] = 0;
  }
  //printf("Память инициализирована\n");
  return 0;
}
int sc_memorySet(int address, int value) {
  if (address >= 0 && address < 100) {
    ram[address] = value;
    //printf("Значение %d установлено в ячейку %d \n", value, address);
    return 0;
  } else {
    //printf("Ошибка: указан неверный адрес\n");
    return 1;
  }
}
int sc_memoryGet(int address, int *value) {
  if (address >= 0 && address < 100) {
    *value = ram[address];
    //printf("Значение записано в value\n");
    return 0;
  } else {
    //printf("Ошибка: указан неверный адрес\n");
    return 1;
  }
}
int sc_memoryShow() {
  printf("\n");
  for (int i = 0; i < 100; i++) {
    printf("%d ", ram[i]);
    if ((i + 1) % 10 == 0)
      printf("\n");
  }
	return 0;
}
int sc_memorySave(char *filename) {
  FILE *fp;
  if (!(fp = fopen(
            filename,
            "wb"))) { //Если же файл не может быть открыт, возвращается NULL.
    fclose(fp);
    //printf("Ошибка: не удалость открыть файл %s \n", filename);
    return 1;
  }
  if (!fwrite(ram, sizeof(int), 100,
              fp)) { //возвращает число записанных элементов
    fclose(fp);
    //printf("Ошибка: не удалось записать информацию в %s \n", filename);
    return 1;
  }
  fclose(fp);
  //printf("Память сохранена в %s \n", filename);
  return 0;
}

int sc_memoryLoad(char *filename) {
  FILE *fp;
  if (!(fp = fopen(filename, "rb"))) {
    fclose(fp);
    //printf("Ошибка: не удалость открыть файл %s \n", filename);
    return 1;
  }
  if (!fread(ram, sizeof(int), 100, fp)) {
    fclose(fp);
    //printf("Ошибка: не удалось считать информацию из %s \n", filename);
    return 1;
  }
  fclose(fp);
  //printf("Память выгружена из %s \n", filename);
  return 0;
}
int sc_regInit() {
  registr = 0;
  //printf("Регистр флагов равен нулю \n");
  return 0;
}
int sc_regSet(int reg, int value) {
  if (value != 0 && value != 1) {
    //printf("Ошибка: значение флага должно быть равно 0 или 1 \n");
    return 1;
  }
  switch (reg) {
  case OVERFLOW:
  case DIVISON_BY_ZERO:
  case GOING_BEYOND_MEMORY:
  case IGNORING_CLOCK_PULSES:
  case WRONG_COMMAND:
    if (value) // 1
      registr = registr | (1 << (reg - 1));
    else // 0
      registr = registr & (~(1 << (reg - 1)));
    break;
  default:
    //printf("Ошибка: указан недопустимый номер регистра \n");
    return 2;
    break;
  }
  //printf("Устанавлено значение указанного регистра флагов \n");
  return 0;
}
int sc_regGet(int reg, int *value) {
  switch (reg) {
  case OVERFLOW:
  case DIVISON_BY_ZERO:
  case GOING_BEYOND_MEMORY:
  case IGNORING_CLOCK_PULSES:
  case WRONG_COMMAND:
    *value = (registr >> (reg - 1)) & 0x1;
    break;
  default:
    //printf("Ошибка: указан недопустимый номер регистра \n");
    return 1;
    break;
  }
  printf("Значение флага сохранено в value \n");
  return 0;
}
int sc_commandEncode(int command, int operand, int *value) {
  // printf("%d", *value);
  if ((command >= 10 && command <= 11) || (command >= 20 && command <= 21) ||
      (command >= 30 && command <= 33) || (command >= 40 && command <= 43) ||
      (command >= 51 && command <= 76)) {
    if ((operand >= 0) && (operand <= 127)) {
      *value = 0;
      *value = *value | (command << 7);
      *value = *value | operand;

    } else {
      //printf("Ошибка: указан неверный операнд \n");
      return 2;
    }
  } else {
    //printf("Ошибка: указана неверная команда \n");
    return 1;
  }
  //printf("Команда успешно закодирована \n");
  return 0;
}
int sc_commandDecode(int value, int *command, int *operand) {
  int temp = 128 - 1; //^0, ^1, ^2, ^3, ^4, ^5, ^6 -- 7 битов
  temp = temp << 7;
  // printf("%d", temp);
  if (value < 16384) // 2^14 = 32768, 15 бит - признак команды
  {
    *command = (value & temp) >> 7;
    printf("%d \n", *command);
    if ((*command >= 10 && *command <= 11) ||
        (*command >= 20 && *command <= 21) ||
        (*command >= 30 && *command <= 33) ||
        (*command >= 40 && *command <= 43) ||
        (*command >= 51 && *command <= 76)) {
      temp = 256 - 1;
      *operand = value & temp;
    } else {
      sc_regSet(WRONG_COMMAND, 1);
      //printf("Ошибка: в value неправильный код операции \n");
      return 2;
    }
  } else {
    sc_regSet(WRONG_COMMAND, 1);
    //printf("Ошибка: value не является командой \n");
    return 1;
  }
  //printf("Код операции сохранен в command, операнд в operand \n");
  return 0;
}
