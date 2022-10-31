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

OBJS=main.o statuschecker.o partitionmanager.o partitioncopyroot.o partitioncopytwrp.o recoverdata.o

all: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)
    
main.o: main.c
	$(CC) -c $(CCFLAGS) main.c -o main.o
    
statuschecker.o: statuschecker.c
	$(CC) -c $(CCFLAGS) statuschecker.c -o statuschecker.o

partitionmanager.o: partitionmanager.c
	$(CC) -c $(CCFLAGS) partitionmanager.c -o partitionmanager.o

partitioncopyroot.o: partitioncopyroot.c
	$(CC) -c $(CCFLAGS) partitioncopyroot.c -o partitioncopyroot.o

partitioncopytwrp.o: partitioncopytwrp.c
	$(CC) -c $(CCFLAGS) partitioncopytwrp.c -o partitioncopytwrp.o

recoverdata.o: recoverdata.c
	$(CC) -c $(CCFLAGS) recoverdata.c -o recoverdata.o

clean:
	rm -f *.o $(TARGET)