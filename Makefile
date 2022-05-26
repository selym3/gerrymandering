

CC := clang++ -std=c++17
WW := # -Wall -Wextra -Wconversion
LL := -lsfml-system -lsfml-graphics -lsfml-window

COMPILER := $(CC) $(WW)
LINKED := $(COMPILER) $(LL)

compile:
	$(COMPILER) -c graphics/engine.cpp -o bin/engine.o
	$(COMPILER) -c graphics/behavior.cpp -o bin/behavior.o
	$(COMPILER) -c algos/simple_map_behavior.cpp -o bin/simple_map_behavior.o
	$(COMPILER) -c algos/simple_map.cpp -o bin/simple_map.o

	$(LINKED) main.cpp bin/*.o -o bin/main

run: compile
	./bin/main