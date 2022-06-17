CXX 	 = g++
LDLIBS   = -lsfml-window -lsfml-graphics -lsfml-system
CXXFLAGS = -std=c++17 -Wall -Wextra -Wconversion

.SUFFIXES = .cpp .o

# define object files necessary for compiling
OBJS = ./bin/behavior.o ./bin/engine.o ./bin/map.o ./bin/map_behavior.o ./bin/metric.o ./bin/node.o # ./bin/simple_map_behavior.o ./bin/simple_map.o

# define dependencies that call for a recompile (can be made more specific to reduce redundant recompiles)
DEPS = ./util/vec2.hpp ./graphics/camera.hpp ./util/Random.hpp

# create output file
all: main
	./main

main: main.cpp $(OBJS) $(DEPS)
	$(CXX) $(CXXFLAGS) $(LDLIBS) main.cpp ./bin/*.o -o main

# clean output
.PHONY: clean

clean:
	rm -rf ./bin
	mkdir ./bin

# define how and when to compile an object file 
# (TODO: replace with a general rule)

./bin/behavior.o : ./graphics/behavior.cpp ./graphics/behavior.hpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o ./bin/$(shell basename $@)

./bin/engine.o : ./graphics/engine.cpp ./graphics/engine.hpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o ./bin/$(shell basename $@)

./bin/map.o : ./algos/map.cpp ./algos/map.hpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o ./bin/$(shell basename $@)

./bin/map_behavior.o : ./algos/map_behavior.cpp ./algos/map_behavior.hpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o ./bin/$(shell basename $@)

./bin/metric.o : ./algos/metric.cpp ./algos/metric.hpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o ./bin/$(shell basename $@)

./bin/node.o : ./algos/node.cpp ./algos/node.hpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o ./bin/$(shell basename $@)
