CC=g++
FLAGS=-O3 -std=c++11
LIB=-lleda

SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(SOURCES:src/%.cpp=build/%.o)

BIN=build/bf

INCL='/usr/include/boost'
LEDA_ROOT=/usr/local/LEDA
LEDA_INCL=$(LEDA_ROOT)/incl

#----------------------------

executable: $(BIN)

$(BIN): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(BIN) $(LIB) -L $(LEDA_ROOT)

build/%.o: src/%.cpp incl/%.h
	$(CC) -c $(FLAGS) -I $(INCL) -I $(LEDA_INCL) $< -o $@

build/main.o: src/main.cpp
	$(CC) -c $(FLAGS) -I $(INCL) -I $(LEDA_INCL) $< -o $@


.PHONY: clean
clean:
	rm build/*.o $(BIN)
