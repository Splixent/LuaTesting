CXX = clang++
CXXFLAGS = -std=c++17 $(shell pkg-config --cflags raylib) -Iengine/vendor $(shell pkg-config --cflags lua)
LDFLAGS = $(shell pkg-config --libs raylib) $(shell pkg-config --libs lua)

all: bin/main

bin/main: engine/main.cpp engine/luau_runtime.cpp engine/bindings_gfx.cpp
	@mkdir -p bin
	$(CXX) engine/main.cpp engine/luau_runtime.cpp engine/bindings_gfx.cpp -o bin/main $(CXXFLAGS) $(LDFLAGS)

run: bin/main
	./bin/main

clean:
	rm -f bin/main
	rmdir bin 2>/dev/null || true

.PHONY: all run clean
