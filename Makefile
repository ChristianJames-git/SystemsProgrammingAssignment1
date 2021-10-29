# CXX Make variable for compiler
CXX=g++
# Make variable for compiler options
#	-std=c++11  C/C++ variant to use, e.g. C++ 2011
#	-g          include information for symbolic debugger e.g. gdb 
CXXFLAGS=-std=c++11 -g

# Rules format:
# target : dependency1 dependency2 ... dependencyN
#     Command to make target, uses default rules if not specified

# First target is the one executed if you just type make
# make target specifies a specific target
# $^ is an example of a special variable.  It substitutes all dependencies
dissem : main.o disass.o Opcode.o
	$(CXX) $(CXXFLAGS) -o dissem $^

main.o : disass.h main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

disass.o : disass.h disass.cpp
	$(CXX) $(CXXFLAGS) -c disass.cpp

Opcode.o : Opcode.h Opcode.cpp
	$(CXX) $(CXXFLAGS) -c Opcode.cpp


clean :
	rm *.o

