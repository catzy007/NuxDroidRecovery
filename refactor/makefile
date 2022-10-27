# change application name here (executable output name)
TARGET=NuxDroidRecovery

# compiler
CC=gcc
# debug
DEBUG=-g
# optimisation
OPT=-O0
# warnings
WARN=-Wall

PTHREAD=-pthread

CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe

# linker
LD=gcc
LDFLAGS=$(PTHREAD) -export-dynamic

OBJS=main.o statuschecker.o partitionselector.o partitioncopy.o

all: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)
    
main.o: main.c
	$(CC) -c $(CCFLAGS) main.c -o main.o
    
statuschecker.o: statuschecker.c
	$(CC) -c $(CCFLAGS) statuschecker.c -o statuschecker.o

partitionselector.o: partitionselector.c
	$(CC) -c $(CCFLAGS) partitionselector.c -o partitionselector.o

partitioncopy.o: partitioncopy.c
	$(CC) -c $(CCFLAGS) partitioncopy.c -o partitioncopy.o

clean:
	rm -f *.o $(TARGET)