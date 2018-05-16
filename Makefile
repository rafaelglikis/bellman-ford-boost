CC=g++
FLAGS=-O3 -std=c++11
LIB=

SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(SOURCES:src/%.cpp=build/%.o)

BIN=build/bf

INCL='/usr/include/boost'

#----------------------------

executable: $(BIN)

$(BIN): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(BIN) $(LIB)

build/%.o: src/%.cpp incl/%.h
	$(CC) -c $(FLAGS) -I $(INCL) $< -o $@

build/main.o: src/main.cpp
	$(CC) -c $(FLAGS) -I $(INCL) $< -o $@


.PHONY: clean
clean:
	rm build/*.o $(BIN)
