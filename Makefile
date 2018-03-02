CXX = g++
CXXFLAGS = -Wall -Wextra -Wpedantic -std=c++11 -O3 -flto -s
CXXFLAGSDEBUG = -Wall -Wextra -Wpedantic -std=c++11 -g
EXEC = rbgParser
OBJECTS = src/options.o src/token.o src/message.o src/slice.o src/macro_bank.o src/slice_iterator.o src/parser_helpers.o src/printer_helpers.o src/straightness_helpers.o src/declarations.o src/game_board.o src/shift.o src/ons.o src/offs.o src/assignments.o src/switch.o src/power_move.o src/star_move.o src/concatenation.o src/modifier_block.o src/sum.o src/conditional_sum.o src/arithmetic_comparison.o src/player_check.o src/condition_check.o src/integer_arithmetic.o src/variable_arithmetic.o src/multiply_arithmetic.o src/sum_arithmetic.o src/game_move.o src/parsed_game.o src/game_items.o src/tree_utils.o src/integer_leaf.o src/identifier_leaf.o src/variable_leaf.o src/arrow_leaf.o src/internal_node.o src/bracketed_expression.o src/tree_parser.o src/typing_machine.o src/rules_parser.o

all: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(EXEC) src/main.cpp $(OBJECTS)

lib: $(OBJECTS)
	$(CXX) -shared -fPIC $(CXXFLAGS) -o $(EXEC).so $(OBJECTS)


debug: $(OBJECTS)
	$(CXX) $(CXXFLAGSDEBUG) -o $(EXEC) src/main.cpp $(OBJECTS)

clean:
	rm -f $(OBJECTS)

distclean: clean
	rm -f $(EXEC)
