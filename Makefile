CXX = clang++
CXXFLAGS = -std=c++17 $(shell pkg-config --cflags raylib)
LDFLAGS = $(shell pkg-config --libs raylib)

all: main

main: main.cpp
	$(CXX) main.cpp -o main $(CXXFLAGS) $(LDFLAGS)

run: main
	./main

clean:
	rm -f main
