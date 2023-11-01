main: lab2.c functions.o broker
	gcc -Wall -g lab2.c functions.o -o lab2 -lm

lab2: lab2.c functions.o
	gcc -Wall -g lab2.c functions.o -o lab2 -lm

broker: broker.c fbroker.o
	gcc -Wall -g broker.c fbroker.o -o broker -lm

fbroker: fbroker.c
	gcc -Wall -g fbroker.c -o fbroker.o

worker: worker.c fworker.o
	gcc -Wall -g worker.c fworker.o -o worker -lm

fworker: fworker.c 
	gcc -Wall -g fworker.c -o fworker.o -lm

functions: functions.c
	gcc -Wall -g functions.c -o functions.o -lm

clean:
	rm -rf *.o main

test_lab2: lab2 broker worker
	./lab2 -N 30000 -P 40 -i test3_30000.txt -o nasho.txt -c 40 -D
