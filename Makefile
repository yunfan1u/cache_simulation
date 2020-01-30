CC = g++

CFLAGS =  -std=c++11 -O3

all: project

project: project.cpp
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f project
