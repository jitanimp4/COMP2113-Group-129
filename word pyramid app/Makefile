CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic

TARGET = word_pyramid
SOURCES = main.cpp word_pyramid.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cpp word_pyramid.h
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
