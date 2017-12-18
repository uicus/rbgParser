#include"pure_concatenation.hpp"

pure_concatenation::pure_concatenation(std::vector<std::unique_ptr<pure_game_move>>&& content):
content(std::move(content)){
}

std::unique_ptr<pure_game_move> pure_concatenation::transform_into_pure(void){
    return std::unique_ptr<pure_game_move>(new pure_concatenation(std::move(content)));
}

void pure_concatenation::accept(abstract_dispatcher& dispatcher){
    dispatcher.dispatch(*this);
}

const std::vector<std::unique_ptr<pure_game_move>>& pure_concatenation::get_content(void)const{
    return content;
}
