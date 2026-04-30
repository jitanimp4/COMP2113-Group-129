CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic

TARGET := odyssey_os

.PHONY: all run clean

all: $(TARGET)

$(TARGET):
	$(CXX) $(CXXFLAGS) \
		"main.cpp" \
		"cryptex_boss.cpp" \
		"wordle.cpp" \
		"crossword/main.cpp" \
		"crossword/game.cpp" \
		"crossword/puzzle.cpp" \
		"word pyramid app/main.cpp" \
		"word pyramid app/word_pyramid.cpp" \
		"The Director's Archive/main.cpp" \
		"The Director's Archive/celluloid_app.cpp" \
		-o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
