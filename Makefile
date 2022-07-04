OBJECTS = sgemm.o
CXXFLAGS = -Ofast -march=native -mtune=native
LDFLAGS = -lm 
CC = g++

.PHONY: all run clean


all: main


main: $(OBJECTS) main.cpp
	$(CC) $(CXXFLAGS) -DUSER main.cpp -c -o build/main.o
	$(CC) build/main.o $(addprefix build/, $(OBJECTS)) $(LDFLAGS) -o main
	@./main

ref-openblas: $(OBJECTS) main.cpp
	$(CC) $(CXXFLAGS) -DOPENBLAS main.cpp -c -o build/main.o
	$(CC) build/main.o $(addprefix build/, $(OBJECTS)) $(LDFLAGS) -lopenblas -o main
	@OMP_NUM_THREADS=1 ./main

ref-mkl: $(OBJECTS) main.cpp
	$(CC) $(CXXFLAGS) -DMKL main.cpp -c -o build/main.o
	$(CC) build/main.o $(addprefix build/, $(OBJECTS)) $(LDFLAGS) -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -o main
	@OMP_NUM_THREADS=1 MKL_NUM_THREADS=1 ./main

ref-baseline: $(OBJECTS) main.cpp
	$(CC) $(CXXFLAGS) -DBASELINE main.cpp -c -o build/main.o
	$(CC) build/main.o $(addprefix build/, $(OBJECTS)) $(LDFLAGS) -o main
	@./main

%.o : src/%.cpp
	mkdir -p build/
	$(CC) -c $(CXXFLAGS) -I. $< -o build/$@

clean:
	rm -rf build/
	rm -rf main