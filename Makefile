

CC := clang++ -std=c++17
WW := # -Wall -Wextra -Wconversion
LL := -lsfml-system -lsfml-graphics -lsfml-window

COMPILER := $(CC) $(WW)
LINKED := $(COMPILER) $(LL)

compile:
	$(COMPILER) -c graphics/engine.cpp -o bin/engine.o
	$(COMPILER) -c graphics/behavior.cpp -o bin/behavior.o
	$(COMPILER) -c algos/map_behavior.cpp -o bin/map_behavior.o
	$(COMPILER) -c algos/map.cpp -o bin/map.o

	$(LINKED) main.cpp bin/*.o -o bin/main

run: compile
	./bin/main