#include"pure_bracketed_move.hpp"

pure_bracketed_move::pure_bracketed_move(void):
contained_move(),
number_of_repetitions(1){
}

pure_bracketed_move::pure_bracketed_move(std::unique_ptr<pure_game_move> contained_move, uint number_of_repetitions):
contained_move(std::move(contained_move)),
number_of_repetitions(number_of_repetitions){
}

std::unique_ptr<pure_game_move> pure_bracketed_move::transform_into_pure(void){
    return std::unique_ptr<pure_game_move>(new pure_bracketed_move(std::move(contained_move),number_of_repetitions));
}
