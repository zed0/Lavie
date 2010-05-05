
CC=g++
CFLAGS=-c
LDFLAGS=
SOURCES=*.cpp plugins/*.cpp
OBJECTS=$(SOURCES: .cpp=.o)
EXECUTABLE=Lavie

all: $(SOURCES) $(EXECUTABLE)
		
$(EXECUTABLE): $(OBJECTS)
		$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
		$(CC) $(CFLAGS) $< -o $@

clean:
		rm Lavie *.o
