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
	$(CC)  $(CFLAGS) src/holes.c src/LinkedListAPI.c -o bin/holes

runDine:
	./bin/dine 3 2

runHoles:
	./bin/holes ./assets/data2.txt

valgrind:
	valgrind --leak-check=full ./bin/holes ./assets/data2.txt

clean:
	rm bin/*

git:
	git add -A
	git commit -m "[AUTO]"
	git push
