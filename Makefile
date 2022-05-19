



compile:
	clang++ -c graphics/engine.cpp -o bin/engine.o
	clang++ -c graphics/behavior.cpp -o bin/behavior.o

	clang++ main.cpp bin/*.o -o bin/main -lsfml-system -lsfml-graphics -lsfml-window

run: compile
	./bin/main