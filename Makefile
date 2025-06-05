# Makefile for CloverSort project

# Compiler and flags
CXX      := g++
CXXFLAGS := -Wall -Iinclude `root-config --cflags`
LDFLAGS  := `root-config --libs`

# Directories
SRC_DIR  := src
OBJ_DIR  := obj
BIN_DIR  := bin

# Target executable name (assumed main from CloverSort.cpp)
TARGET   := $(BIN_DIR)/CloverSort

# Find all source files in the src directory
SOURCES  := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS  := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# Default target
all: $(TARGET)

# Link object files into the executable
$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean