OBJECTS = sgemm.o
CXXFLAGS = -Ofast -march=native -mtune=native
LDFLAGS = -lm
CC = g++

.PHONY: all run clean


all: run


run: $(OBJECTS) main
	@./main

main: $(OBJECTS) main.cpp
	$(CC) $(CXXFLAGS) main.cpp -c -o main.o
	$(CC) main.o $(OBJECTS) $(LDFLAGS) -o main


%.o : src/%.cpp
	$(CC) -c $(CXXFLAGS) -I. $<

clean:
	rm -rf *.o
	rm -rf main