CXX = g++
CXXFLAGS = -Wall -Wextra -Wpedantic -std=c++11 -O3 -flto -s
CXXFLAGSDEBUG = -Wall -Wextra -Wpedantic -std=c++11 -g
EXEC = rbg2gdl
OBJECTS = src/options.o src/token.o src/message.o src/slice.o src/macro_bank.o src/slice_iterator.o src/parser_helpers.o src/declarations.o src/game_board.o src/shift.o src/ons.o src/offs.o src/assignments.o src/pure_bracketed_move.o src/bracketed_move.o src/pure_concatenation.o src/concatenation.o src/pure_sum.o src/sum.o src/move_condition.o src/comparison.o src/negatable_condition.o src/conjunction.o src/alternative.o src/condition_check.o src/parsed_game.o src/game_items.o

all: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(EXEC) src/main.cpp $(OBJECTS)

debug: $(OBJECTS)
	$(CXX) $(CXXFLAGSDEBUG) -o $(EXEC) src/main.cpp $(OBJECTS)

clean:
	rm -f $(OBJECTS)

distclean: clean
	rm -f $(EXEC)
