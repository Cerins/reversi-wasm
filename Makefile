SRC = src/main.cpp src/reversi.cpp

TEST_SRC = src/tests/test.cpp

TARGET_DIR = dist

TARGET = $(TARGET_DIR)/index

TEST_TARGET  = test

CC = emcc

CXX = g++

CFLAGS = -O3 -s WASM=1 -s EXPORTED_RUNTIME_METHODS='["cwrap"]'

CXXFLAGS = -std=c++17

LFLAGS = -s NO_EXIT_RUNTIME=1 

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(LFLAGS) -o $(TARGET).js $(SRC)

tests: $(TEST_SRC)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_SRC)
	./$(TEST_TARGET)
	rm -f $(TEST_TARGET)

.PHONY: clean

.PHONY: serve

# Create a python server to serve the files within target directory
# This is needed because browsers do not allow to load wasm files without proper CORS
serve:
	python3 -m http.server 8080 --directory $(TARGET_DIR)


clean:
	rm -f $(TARGET).wasm