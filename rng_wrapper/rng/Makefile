BINDIR?=./bin
LDLIBS+=-lm
CC=gcc ${DBG} -O ${TRACE} -Wall -Wpedantic
PROGS= rngstart rngstub


all: $(PROGS)
rngstart: rngstart.o xoshiro256plus.o rngctrl.o 
rngstub: rngstub.o xoshiro256plus.o rngctrl.o 

rngstart.o:: rngctrl.h xoshiro256plus.h
rngstub.o:: rngctrl.h xoshiro256plus.h
xoshiro256plus.o:: xoshiro256plus.h

clean:
	- rm *.o *.log ${PROGS}
	
install:
	mkdir -p ${BINDIR}
	cp ${PROGS} ${BINDIR}


