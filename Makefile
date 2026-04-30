CXX      = g++
CXXFLAGS = -std=c++17 -O2 -Wall
LDFLAGS  = -lpthread

# The final name of your main game
TARGET   = odyssey_os

# Automatically find ALL .cpp files in the root folder AND all sub-folders
# (This is why your folder names CANNOT have spaces!)
SRCS     = $(shell find . -type f -name '*.cpp')

# Convert the list of .cpp files into a list of .o (object) files
OBJS     = $(SRCS:.cpp=.o)

# Default rule
all: $(TARGET)

# Link all the object files together to create the final game
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Rule to compile each .cpp file into a .o file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Cleanup rule to remove old compiled files and saves
clean:
	rm -f $(TARGET) $(OBJS) save_data.txt

.PHONY: all clean
