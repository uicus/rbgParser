#include"prioritized_sum.hpp"
#include"printer_helpers.hpp"

namespace rbg_parser{

prioritized_sum::prioritized_sum(std::vector<std::unique_ptr<game_move>>&& content):
content(std::move(content)){
}

int prioritized_sum::give_indices_in_expression(int next_free){
    for(const auto& el: content)
        next_free = el->give_indices_in_expression(next_free);
    return next_free;
}

bool prioritized_sum::modifies(void)const{
    for(const auto& el: content)
        if(el->modifies())
            return true;
    return false;
}

std::unique_ptr<game_move> prioritized_sum::simplify(void){
    std::vector<std::unique_ptr<game_move>> result;
    for(uint i=0;i<content.size();++i)
        result.push_back(content[i]->simplify());
    if(result.size()==1)
        return std::move(result[0]);
    else
        return std::unique_ptr<game_move>(new prioritized_sum(std::move(result)));
}

void prioritized_sum::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

std::string prioritized_sum::to_rbg(uint indent)const{
    std::string result = "";
    for(uint i=0;i<content.size();++i){
        result += (i==0 ? "\n":"\n+> ");
        result += open_bracket_if_necessary(priority(),content[i]->priority());
        result += content[i]->to_rbg(indent+1);
        result += close_bracket_if_necessary(priority(),content[i]->priority());
    }
    result += "\n";//+print_tabs(indent);
    return result;
}

std::string prioritized_sum::to_rbg()const{
    std::string result = "";
    for(uint i=0;i<content.size();++i){
        result += (i==0 ? "" : " + ");
        result += open_bracket_if_necessary(priority(),content[i]->priority());
        result += content[i]->to_rbg();
        result += close_bracket_if_necessary(priority(),content[i]->priority());
    }
    return result;
}

std::unique_ptr<game_move> prioritized_sum::flatten(void){
    std::vector<std::unique_ptr<game_move>> result;
    for(auto& el: content)
        result.push_back(el->flatten());
    return std::unique_ptr<game_move>(new prioritized_sum(std::move(result)));
}

straightness_result prioritized_sum::compute_k_straightness(void)const{
    auto current_max = empty_move();
    for(const auto& el: content)
        current_max.max_of_results(el->compute_k_straightness());
    return current_max;
}

const std::vector<std::unique_ptr<game_move>>& prioritized_sum::get_content(void)const{
    return content;
}

std::unique_ptr<game_move> prioritized_sum::copy(void)const{
    std::vector<std::unique_ptr<game_move>> result_content;
    for(const auto& el: content)
        result_content.push_back(el->copy());
    return std::unique_ptr<game_move>(new prioritized_sum(std::move(result_content)));
}

bool prioritized_sum::has_piece_as_variable(const std::set<token>& pieces)const{
    for(const auto& el: content)
        if(el->has_piece_as_variable(pieces))
            return true;
    return false;
}

}
