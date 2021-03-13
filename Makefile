CC	= g++ -std=c++11
CPPFLAGS= -Wno-deprecated -O3 -c 
LDFLAGS	= -O3 
SOURCES	= main.cpp Graph.cpp 
OBJECTS	= $(SOURCES:.cpp=.o)
EXECUTABLE = temporalGraphGenerator

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o : 
	$(CC) $(CPPFLAGS) $< -o $@

clean:
	rm -f *.o temporalGraphGenerator
