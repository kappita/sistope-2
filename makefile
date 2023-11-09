main: lab2.c functions.o broker worker
	gcc -Wall -g lab2.c functions.o -o lab2 -lm

broker: broker.c fbroker.o
	gcc -Wall -g broker.c fbroker.o -o broker -lm

fbroker: fbroker.c
	gcc -Wall -g fbroker.c -o fbroker.o -lm

worker: worker.c fworker.o
	gcc -Wall -g worker.c fworker.o -o worker -lm

fworker: fworker.c
	gcc -Wall -g fworker.c -o fworker.o -lm

lab2: lab2.c functions.o worker broker
	gcc -Wall -g lab2.c functions.o -o lab2 -lm

functions: functions.c
	gcc -Wall -g functions.c -o functions.o -lm

test_lab2: lab2 worker broker
	./lab2 -N 30000 -P 40 -i test3_30000.txt -o resultados.txt -c 50 -D

clean:
	rm -rf *.o main