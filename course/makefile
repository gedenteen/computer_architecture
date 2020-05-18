COMP = gcc
FLAGS = -Wall -o
OBJS = main.c ../lab1/mySimpleComputer.c ../lab2/myTerm.c ../lab3/myBigChars.c \
    ../lab4/myReadKey.c ../lab5/mySignals.c myProcessor.c

.PHONY: clean run all

all: main

main:
	$(COMP) $(OBJS) $(FLAGS) main.exe

#-include *.d

#main: main.o
#	gcc -c main.o 

#main.o: main.c
#	gcc -o main.c 
