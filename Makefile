# Makefile for SFML project

# Compiler
CC := g++

# Compiler flags
CFLAGS := -std=c++11 -Wall

# SFML libraries
LIBS := -lsfml-graphics -lsfml-window -lsfml-system

# Source files
SOURCES := $(wildcard *.cpp)

# Executable name
EXECUTABLE := SuperMarioClone

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE) $(LIBS)

clean:
	rm -f $(EXECUTABLE)
