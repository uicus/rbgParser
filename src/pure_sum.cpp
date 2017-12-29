#include"pure_sum.hpp"
#include"printer_helpers.hpp"

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

std::string pure_sum::to_rbg(uint indent)const{
    std::string result = "";
    for(uint i=0;i<content.size();++i){
        result += "\n"+(i==0 ? print_tabs(indent+1) : print_tabs_with_half_full_end(indent+1)+"+ ");
        result += open_bracket_if_necessary(priority(),content[i]->priority());
        result += content[i]->to_rbg(indent+1);
        result += close_bracket_if_necessary(priority(),content[i]->priority());
    }
    result += "\n"+print_tabs(indent);
    return result;
}

std::string pure_sum::to_rbg()const{
    std::string result = "";
    for(uint i=0;i<content.size();++i){
        result += (i==0 ? "" : " + ");
        result += open_bracket_if_necessary(priority(),content[i]->priority());
        result += content[i]->to_rbg();
        result += close_bracket_if_necessary(priority(),content[i]->priority());
    }
    return result;
}

const std::vector<std::unique_ptr<pure_game_move>>& pure_sum::get_content(void)const{
    return content;
}
