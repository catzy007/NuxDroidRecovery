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

OBJS=main.o statuschecker.o partitionmgrroot.o partitionmgrtwrp.o

all: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)
    
main.o: main.c
	$(CC) -c $(CCFLAGS) main.c -o main.o
    
statuschecker.o: statuschecker.c
	$(CC) -c $(CCFLAGS) statuschecker.c -o statuschecker.o

partitionmgrroot.o: partitionmgrroot.c
	$(CC) -c $(CCFLAGS) partitionmgrroot.c -o partitionmgrroot.o

partitionmgrtwrp.o: partitionmgrtwrp.c
	$(CC) -c $(CCFLAGS) partitionmgrtwrp.c -o partitionmgrtwrp.o


clean:
	rm -f *.o $(TARGET)