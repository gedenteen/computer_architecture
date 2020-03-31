#include "mySimpleComputer.h"

int main() {

	int answ = 1, address, value;
	int command, operand;
	while (answ) 
	{
		printf("Введите номер команды. '1' - информация о командах, '0' - выход. \n");
		scanf("%d", &answ);
		switch (answ) 
		{
			case 1:
				printf("1. Информация о командах\n");
				printf("2. sc_memoryInit()\n");
				printf("3. sc_memorySet(int address, int value)\n");
				printf("4. sc_memoryGet(int address, int * value)\n");
				printf("5. Показать все ячейки памяти \n");
				printf("6. Вывести значение value \n");
				printf("7. sc_memorySave(char * filename) \n");
				printf("8. sc_memoryLoad(char * filename) \n");
				printf("9. int sc_regInit()\n");
				printf("10. int sc_regSet(int reg, int value) \n");
				printf("11. Вывести значение registr \n");
				printf("12. int sc_regGet (int reg, int * value) \n");
				printf("13. int sc_commandEncode (int command, int operand, int * value) \n");
				printf("14. int sc_commandDecode (int value, int * command, int * operand) \n");
				printf("15. Вывести значение command и операнд \n");
				printf("0. Выход \n");
				break;
			case 2:
				sc_memoryInit();
				break;
			case 3:
				printf("address and value: ");
				scanf("%d %d", &address, &value);
				sc_memorySet(address, value);
				break;
			case 4:
				printf("address: ");
				scanf("%d", &address);
				sc_memoryGet(address, &value);
				break;
			case 5:
				sc_memoryShow();
				break;
			case 6:
				printf("value == %d \n", value);
				break;
			case 7:
				sc_memorySave("file.dat");
				break;
			case 8:
				sc_memoryLoad("file.dat");
				break;
			case 9: 
				sc_regInit();
				break;
			case 10:
				printf("reg and value: ");
				scanf("%d %d", &address, &value);
				sc_regSet(address, value);
				break;
			case 11:
				printf("registr == %d \n", registr);
				break;
			case 12:
				printf("reg: ");
				scanf("%d", &address);
				sc_regGet (address, &value);
				break;
			case 13:
				printf("command and operand: ");
				scanf("%d %d", &command, &operand);
				sc_commandEncode(command, operand, &value);
				break;
			case 14:
				printf("value: ");
				scanf("%d", &value);		
				sc_commandDecode (value, &command, &operand);
				break;
			case 15:
				printf("command == %d, operand == %d \n", command, operand);
				break;
			default:
				break;
		}

	}

	return 0;
}
