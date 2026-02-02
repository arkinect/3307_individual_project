# Compiler and tool configuration
CXX = g++
CXXFLAGS = `wx-config --cxxflags` -std=c++17 -g -Wall
LIBS = `wx-config --libs`

# Target executable name
TARGET = FileManager

# Source and object files
SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)

# Default rule to build the project
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

# Rule to compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean