#include"pure_sum.hpp"

pure_sum::pure_sum(std::vector<std::unique_ptr<pure_game_move>>&& content):
content(std::move(content)){
}

std::unique_ptr<pure_game_move> pure_sum::transform_into_pure(void){
    return std::unique_ptr<pure_game_move>(new pure_sum(std::move(content)));
}
