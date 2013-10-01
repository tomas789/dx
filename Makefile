CXXFLAGS = -Wall -pedantic -std=c++11 -Wno-tautological-compare
SOURCES = exparser.cpp expression.cpp main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = dx

.PHONY: all clean distclean

clean_list = $(OBJECTS) $(EXECUTABLE)

all: $(EXECUTABLE)

clean: 
	$(RM) -rf $(clean_list)

distclean: clean

clean_list += ${SOURCES:.cpp=.d}

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXECUTABLE)

%.d: $(SOURCES)
	$(CXX) $(CXXFLAGS) -MM $? > $@

-include ${SOURCES:.cpp=.d}

# .cpp.o:
# 	$(CXX) $(CXXFLAGS) -c $<
