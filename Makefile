# Makefile for CloverSort project

# Compiler and flags
CXX      := g++
CXXFLAGS := -Wall -fPIC -Iinclude `root-config --cflags` # Added -fPIC
LDFLAGS  := `root-config --libs`

# Directories
SRC_DIR  := src
OBJ_DIR  := obj
BIN_DIR  := bin
LIB_DIR  := lib
INC_DIR := include

# Target executable name (assumed main from CloverSort.cpp)
TARGET   := $(BIN_DIR)/CloverSort

# Find all source files in the src directory
SOURCES  := $(wildcard $(SRC_DIR)/*.cpp)
# All object files
OBJECTS  := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# Shared library components
# Sources for the shared library (all .cpp files in SRC_DIR except CloverSort.cpp)
LIB_SOURCES := $(filter-out $(SRC_DIR)/CloverSort.cpp, $(SOURCES))
# Object files for the shared library (these are a subset of OBJECTS)
LIB_OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(LIB_SOURCES))
# Shared library target name
SHARED_LIB_TARGET := $(LIB_DIR)/libCloverSort.so

# Default target
all: $(TARGET)

# Target to build the shared library
lib: $(SHARED_LIB_TARGET)

# Link object files into the executable
$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Rule to create the shared library
$(SHARED_LIB_TARGET): $(LIB_OBJECTS)
	@mkdir -p $(LIB_DIR)
	$(CXX) -shared -o $@ $^ $(LDFLAGS)

# Compile source files into object files
# This rule is used for all .o files, which will be PIC due to -fPIC in CXXFLAGS
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

install: $(TARGET) $(SHARED_LIB_TARGET)
	@mkdir -p ~/.local/bin
	@cp $(TARGET) ~/.local/bin/
	@mkdir -p ~/.local/lib
	@cp $(SHARED_LIB_TARGET) ~/.local/lib
	@mkdir -p ~/.local/include/CloverSort
	@cp $(INC_DIR)/*.hpp ~/.local/include/CloverSort

uninstall:
	sudo rm -f ~/.local/bin/CloverSort
	sudo rm -rf ~/.local/lib/CloverSort
	sudo rm -rf ~/.local/include/CloverSort

# Clean up build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean library