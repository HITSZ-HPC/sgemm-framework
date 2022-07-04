OBJECTS = sgemm.o
CXXFLAGS = -Ofast -march=native -mtune=native
LDFLAGS = -lm 
CC = g++

.PHONY: all run clean


all: run


run: $(OBJECTS) main
	@./main

main: $(OBJECTS) main.cpp
	$(CC) $(CXXFLAGS) -DUSER main.cpp -c -o main.o
	$(CC) main.o $(OBJECTS) $(LDFLAGS) -o main

ref-openblas: $(OBJECTS) main.cpp
	$(CC) $(CXXFLAGS) -DOPENBLAS main.cpp -c -o main.o
	$(CC) main.o $(OBJECTS) $(LDFLAGS) -lopenblas -o main
	@OMP_NUM_THREADS=1 ./main

ref-mkl: $(OBJECTS) main.cpp
	$(CC) $(CXXFLAGS) -DMKL main.cpp -c -o main.o
	$(CC) main.o $(OBJECTS) $(LDFLAGS) -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -o main
	@OMP_NUM_THREADS=1 MKL_NUM_THREADS=1 ./main

ref-baseline: $(OBJECTS) main.cpp
	$(CC) $(CXXFLAGS) -DBASELINE main.cpp -c -o main.o
	$(CC) main.o $(OBJECTS) $(LDFLAGS) -o main
	@./main

%.o : src/%.cpp
	$(CC) -c $(CXXFLAGS) -I. $<

clean:
	rm -rf *.o
	rm -rf main