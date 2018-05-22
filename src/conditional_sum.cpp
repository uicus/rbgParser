#include"conditional_sum.hpp"
#include"printer_helpers.hpp"
#include<algorithm>

namespace rbg_parser{

conditional_sum::conditional_sum(std::vector<std::unique_ptr<game_move>>&& content, bool reverse):
content(std::move(content)){
    if(reverse)
        std::reverse(content.begin(),content.end());
}

bool conditional_sum::modifies(void)const{
    for(const auto& el: content)
        if(el->modifies())
            return true;
    return false;
}

std::unique_ptr<game_move> conditional_sum::simplify(void){
    std::vector<std::unique_ptr<game_move>> result;
    for(uint i=0;i<content.size();++i)
        result.push_back(content[i]->simplify());
    if(result.size()==1)
        return std::move(result[0]);
    else
        return std::unique_ptr<game_move>(new conditional_sum(std::move(result)));
}

void conditional_sum::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

std::string conditional_sum::to_rbg(uint indent)const{
    std::string result = "";
    for(uint i=0;i<content.size();++i){
        result += "\n"+(i==0 ? print_tabs(indent+1) : print_tabs(indent)+"  > ");
        result += open_bracket_if_necessary(priority(),content[i]->priority());
        result += content[i]->to_rbg(indent+1);
        result += close_bracket_if_necessary(priority(),content[i]->priority());
    }
    result += "\n"+print_tabs(indent);
    return result;
}

std::string conditional_sum::to_rbg()const{
    std::string result = "";
    for(uint i=0;i<content.size();++i){
        result += (i==0 ? "" : " > ");
        result += open_bracket_if_necessary(priority(),content[i]->priority());
        result += content[i]->to_rbg();
        result += close_bracket_if_necessary(priority(),content[i]->priority());
    }
    return result;
}

std::unique_ptr<game_move> conditional_sum::flatten(void){
    std::vector<std::unique_ptr<game_move>> result;
    for(auto& el: content)
        result.push_back(el->flatten());
    return std::unique_ptr<game_move>(new conditional_sum(std::move(result)));
}

straightness_result conditional_sum::compute_k_straightness(void)const{
    auto current_max = empty_move();
    for(const auto& el: content)
        current_max.max_of_results(el->compute_k_straightness());
    return current_max;
}

const std::vector<std::unique_ptr<game_move>>& conditional_sum::get_content(void)const{
    return content;
}

}

