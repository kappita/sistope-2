main: lab1.c functions.o
	gcc -Wall -g lab1.c functions.o -o lab1 -lm

lab1: lab1.c functions.o
	gcc -Wall -g lab1.c functions.o -o lab1 -lm

functions: functions.c
	gcc -Wall -g functions.c -o functions.o -lm

clean:
	rm -rf *.o main