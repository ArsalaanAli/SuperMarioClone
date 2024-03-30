# Makefile for SFML project

# Compiler
CC := g++

# Compiler flags
CFLAGS := -std=c++20 -Wall

# SFML libraries
LIBS := -lsfml-graphics -lsfml-window -lsfml-system
LIB_PATHS := -L/opt/homebrew/Cellar/sfml/2.6.1/lib
INCLUDES := -I/opt/homebrew/Cellar/sfml/2.6.1/include

# Source files
SOURCES := $(wildcard src/*.cpp)

# Executable name
EXECUTABLE := SuperMarioClone

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE) $(LIBS) $(LIB_PATHS) $(INCLUDES)

clean:
	rm -f $(EXECUTABLE)
