



compile:
	clang++ -std=c++17 -c graphics/engine.cpp -o bin/engine.o
	clang++ -std=c++17 -c graphics/behavior.cpp -o bin/behavior.o
	clang++ -std=c++17 -c algos/map_behavior.cpp -o bin/map_behavior.o

	clang++ -std=c++17 main.cpp bin/*.o -o bin/main -lsfml-system -lsfml-graphics -lsfml-window

run: compile
	./bin/main