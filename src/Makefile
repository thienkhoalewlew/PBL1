# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall

# Source files and target
SOURCES = Main.cpp utils.cpp transportationModel.cpp initialSolution.cpp optimization.cpp ui.cpp solver.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = Program.exe

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJECTS) $(TARGET)

# Dependencies
Main.o: Main.cpp utils.h transportationModel.h initialSolution.h optimization.h ui.h solver.h
utils.o: utils.cpp utils.h
transportationModel.o: transportationModel.cpp transportationModel.h
initialSolution.o: initialSolution.cpp initialSolution.h
optimization.o: optimization.cpp optimization.h
ui.o: ui.cpp ui.h
solver.o: solver.cpp solver.h

.PHONY: all clean
