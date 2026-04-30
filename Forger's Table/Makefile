CXX      = g++
CXXFLAGS = -std=c++17 -O2 -Wall
LDFLAGS  = -lpthread

TARGET   = forger_table
SRC      = main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET) save_data.txt

.PHONY: all clean
