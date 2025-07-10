# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Iinclude
LDFLAGS = -Llib -lraylib -lopengl32 -lgdi32 -lwinmm

# File structure
SRC_DIR = src
OBJ_DIR = obj
BIN = launcher.exe

# Gather source and object files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Default target
all: $(BIN)

# Link objects into final executable
$(BIN): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Compile each .cpp file into .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# Create object directory if it doesn't exist
$(OBJ_DIR):
	mkdir $(OBJ_DIR)

# Clean build files
clean:
	del /Q $(OBJ_DIR)\*.o $(BIN)

.PHONY: all clean