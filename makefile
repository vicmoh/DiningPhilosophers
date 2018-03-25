CC = gcc
CFLAGS = -Wall -g -Iinclude

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
	CCFLAGS += -std=gnu11 
endif
ifeq ($(UNAME), Darwin)
	CCFLAGS += -std=c11
endif

all: dine holes

dine:  
	$(CC)  $(CFLAGS) src/dine.c -o bin/dine -pthread

holes:
	$(CC)  $(CFLAGS) src/holes.c -o bin/holes -lm

valgrind:
	valgrind -v --leak-check=full ./bin/run

clean:
	rm bin/*