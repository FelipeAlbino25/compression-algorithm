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

clean-tests:
	rm -f ./examples/outputs/huff/*
	rm -f ./examples/outputs/huff-naive/*
	rm -f ./examples/outputs/red/*

tests:
	make
	./compact ./examples/rtext.txt -redundancy
	./compact ./examples/rtext.txt -huffman-naive
	./compact ./examples/rtext.txt -huffman
	
	mv ./examples/rtext.txt.freg ./examples/outputs/red
	mv ./examples/rtext.txt.huff.freg ./examples/outputs/huff-naive
	mv ./examples/rtext.txt.hopt.freg ./examples/outputs/huff

	./compact ./examples/testbench.txt -redundancy
	./compact ./examples/testbench.txt -huffman-naive
	./compact ./examples/testbench.txt -huffman
	
	mv ./examples/testbench.txt.freg ./examples/outputs/red
	mv ./examples/testbench.txt.huff.freg ./examples/outputs/huff-naive
	mv ./examples/testbench.txt.hopt.freg ./examples/outputs/huff

	./compact ./examples/text.txt -redundancy
	./compact ./examples/text.txt -huffman-naive
	./compact ./examples/text.txt -huffman
	
	mv ./examples/text.txt.freg ./examples/outputs/red
	mv ./examples/text.txt.huff.freg ./examples/outputs/huff-naive
	mv ./examples/text.txt.hopt.freg ./examples/outputs/huff

	./compact ./examples/image.bmp -redundancy
	./compact ./examples/image.bmp -huffman-naive
	./compact ./examples/image.bmp -huffman
	
	mv ./examples/image.bmp.freg ./examples/outputs/red
	mv ./examples/image.bmp.huff.freg ./examples/outputs/huff-naive
	mv ./examples/image.bmp.hopt.freg ./examples/outputs/huff

	./compact ./examples/himage.bmp -redundancy
	./compact ./examples/himage.bmp -huffman-naive
	./compact ./examples/himage.bmp -huffman
	
	mv ./examples/himage.bmp.freg ./examples/outputs/red
	mv ./examples/himage.bmp.huff.freg ./examples/outputs/huff-naive
	mv ./examples/himage.bmp.hopt.freg ./examples/outputs/huff

	./compact ./examples/himage2.bmp -redundancy
	./compact ./examples/himage2.bmp -huffman-naive
	./compact ./examples/himage2.bmp -huffman
	
	mv ./examples/himage2.bmp.freg ./examples/outputs/red
	mv ./examples/himage2.bmp.huff.freg ./examples/outputs/huff-naive
	mv ./examples/himage2.bmp.hopt.freg ./examples/outputs/huff

