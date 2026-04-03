#compiler name
CXX = g++

# compilation flags (-Wall show util warnings, -std=c++17 for modern standards)
CXXFLAGS = -Wall -Wextra -std=c++17

# final executable name
TARGET = compact

# collects all .cpp files from /logic /testbenches and the source
SRCS = main.cpp $(wildcard logic/*.cpp) $(wildcard testbenches/*.cpp)

# transform the list of .cpp files into file objects
OBJS = $(SRCS:.cpp=.o)

# main rule: default 'make' command
all: $(TARGET)

# building final executable, gathering all file objects
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

#generic as to how to compile each .cpp file individually into a .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean the mess after the compilation -> type command 'make clean'
clean:
	rm -f $(OBJS) $(TARGET)