CXXFLAGS = -Wall -pedantic -std=c++11 -Wno-tautological-compare
OUTPUT = dx
OBJS = exparser.o expression.o main.o

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

all : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(OUTPUT) $?

clean : 
	rm -rf $(OUTPUT) $(OBJS)

.PHONY = all clean
