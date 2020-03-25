CC = g++
CFLAGS = -c

all: worker.out load_balancer.out

load_balancer.out: main.o  LoadBalancer.o Command.o
	$(CC) -o load_balancer.out main.o LoadBalancer.o Command.o

worker.out: worker.o Command.o
	$(CC) -o worker.out worker.o Command.o

LoadBalancer.o: LoadBalancer.cpp LoadBalancer.h
	$(CC) $(CFLAGS) LoadBalancer.cpp

Command.o: Command.cpp Command.h
	$(CC) $(CFLAGS) Command.cpp


worker.o: worker.cpp Command.h
	$(CC) $(CFLAGS) worker.cpp

clean:
	rm *.o
	rm load_balancer.out
	rm worker.out
