#include"pure_sum.hpp"

pure_sum::pure_sum(std::vector<std::unique_ptr<pure_game_move>>&& content):
content(std::move(content)){
}

std::unique_ptr<pure_game_move> pure_sum::transform_into_pure(void){
    return std::unique_ptr<pure_game_move>(new pure_sum(std::move(content)));
}

std::unique_ptr<pure_game_move> pure_sum::pure_simplify(void){
    std::vector<std::unique_ptr<pure_game_move>> result;
    for(uint i=0;i<content.size();++i)
        result.push_back(content[i]->pure_simplify());
    if(result.size()==1)
        return std::move(result[0]);
    else
        return std::unique_ptr<pure_game_move>(new pure_sum(std::move(result)));
}

void pure_sum::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

const std::vector<std::unique_ptr<pure_game_move>>& pure_sum::get_content(void)const{
    return content;
}
