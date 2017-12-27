#include"pure_concatenation.hpp"

pure_concatenation::pure_concatenation(std::vector<std::unique_ptr<pure_game_move>>&& content):
content(std::move(content)){
}

std::unique_ptr<pure_game_move> pure_concatenation::transform_into_pure(void){
    return std::unique_ptr<pure_game_move>(new pure_concatenation(std::move(content)));
}

std::unique_ptr<pure_game_move> pure_concatenation::pure_simplify(void){
    std::vector<std::unique_ptr<pure_game_move>> result;
    for(uint i=0;i<content.size();++i)
        result.push_back(content[i]->pure_simplify());
    if(result.size()==1)
        return std::move(result[0]);
    else
        return std::unique_ptr<pure_game_move>(new pure_concatenation(std::move(result)));
}

void pure_concatenation::accept(abstract_dispatcher& dispatcher){
    dispatcher.dispatch(*this);
}

const std::vector<std::unique_ptr<pure_game_move>>& pure_concatenation::get_content(void)const{
    return content;
}
