



compile:
	clang++ -c graphics/engine.cpp -o bin/engine.o
	
	clang++ main2.cpp bin/*.o -o bin/main -lsfml-system -lsfml-graphics -lsfml-window

run: compile
	./bin/main