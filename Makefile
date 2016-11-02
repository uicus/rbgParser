CXX = g++
CXXFLAGS = -Wall -Wextra -Wpedantic -std=c++11 -O3 -flto -s
CXXFLAGSDEBUG = -Wall -Wextra -Wpedantic -std=c++11 -g
EXEC = rbg2gdl
OBJECTS = src/options.o src/token.o src/message.o src/slice.o src/macro_bank.o src/slice_iterator.o src/game_board.o src/parsed_game.o src/game_items.o

all: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(EXEC) src/main.cpp $(OBJECTS)

debug: $(OBJECTS)
	$(CXX) $(CXXFLAGSDEBUG) -o $(EXEC) src/main.cpp $(OBJECTS)

clean:
	rm -f $(OBJECTS)

distclean: clean
	rm -f $(EXEC)
