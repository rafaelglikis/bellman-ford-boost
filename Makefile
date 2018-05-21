CC=g++
FLAGS=-O3 -std=c++0x
LIB=-lleda

SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(SOURCES:src/%.cpp=build/%.o)

BIN=bin/bf
INCL='/usr/include/boost'
LEDA_ROOT=/usr/local/LEDA
LEDA_INCL=$(LEDA_ROOT)/incl

exe: $(BIN)

$(BIN): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(BIN) $(LIB) -L $(LEDA_ROOT)

build/%.o: src/%.cpp incl/%.h
	$(CC) -c $(FLAGS) -I $(INCL) -I $(LEDA_INCL) $< -o $@

build/main.o: src/main.cpp
	$(CC) -c $(FLAGS) -I $(INCL) -I $(LEDA_INCL) $< -o $@

clean:
	rm build/*.o $(BIN)
