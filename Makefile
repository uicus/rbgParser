CXX = g++
CXXFLAGS = -Wall -Wextra -Wpedantic -std=c++11 -O3 -flto -s
CXXFLAGSDEBUG = -Wall -Wextra -Wpedantic -std=c++11 -g
EXEC = rbg2gdl
OBJECTS = src/options.o src/token.o src/message.o src/slice.o src/macro_bank.o src/slice_iterator.o src/parser_helpers.o src/declarations.o src/game_board.o src/shift.o src/ons.o src/bracketed_move.o src/parsed_game.o src/game_items.o

all: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(EXEC) src/main.cpp $(OBJECTS)

debug: $(OBJECTS)
	$(CXX) $(CXXFLAGSDEBUG) -o $(EXEC) src/main.cpp $(OBJECTS)

clean:
	rm -f $(OBJECTS)

distclean: clean
	rm -f $(EXEC)
