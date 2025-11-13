CXX = clang++
CXXFLAGS = -std=c++17 $(shell pkg-config --cflags raylib)
LDFLAGS = $(shell pkg-config --libs raylib)

all: bin/main

bin/main: engine/main.cpp
	@mkdir -p bin
	$(CXX) engine/main.cpp -o bin/main $(CXXFLAGS) $(LDFLAGS)

run: bin/main
	./bin/main

clean:
	rm -f bin/main
	rmdir bin 2>/dev/null || true

.PHONY: all run clean
