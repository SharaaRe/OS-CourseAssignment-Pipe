CC = g++ -g
CFLAGS = -c 

all: worker.out load_balancer.out representer.out

load_balancer.out: main.o  LoadBalancer.o Command.o
	$(CC) -o load_balancer.out main.o LoadBalancer.o Command.o

worker.out: worker.o Command.o files.o
	$(CC) -o worker.out worker.o Command.o files.o

representer.out: representer.o Command.o files.o
	$(CC) -o representer.out representer.o Command.o files.o


# OBJ FILES

LoadBalancer.o: LoadBalancer.cpp LoadBalancer.h Command.h
	$(CC) $(CFLAGS) LoadBalancer.cpp

Command.o: Command.cpp Command.h
	$(CC) $(CFLAGS) Command.cpp


worker.o: worker.cpp Command.h files.h
	$(CC) $(CFLAGS) worker.cpp

representer.o: representer.cpp Command.h files.h
	$(CC) $(CFLAGS) representer.cpp

files.o: files.cpp files.h
	$(CC) $(CFLAGS) files.cpp

clean:
	rm *.o
	rm load_balancer.out
	rm worker.out
	rm representer.out
