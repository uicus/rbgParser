#include"concatenation.hpp"
#include"printer_helpers.hpp"
#include"modifier_block.hpp"

namespace rbg_parser{

concatenation::concatenation(std::vector<std::unique_ptr<game_move>>&& content):
content(std::move(content)){
}

bool concatenation::modifies(void)const{
    for(const auto& el: content)
        if(el->modifies())
            return true;
    return false;
}

std::unique_ptr<game_move> concatenation::simplify(void){
    std::vector<std::unique_ptr<game_move>> result;
    for(uint i=0;i<content.size();++i)
        result.push_back(content[i]->simplify());
    if(result.size()==1)
        return std::move(result[0]);
    else
        return std::unique_ptr<game_move>(new concatenation(std::move(result)));
}

void concatenation::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

std::string concatenation::to_rbg(uint indent)const{
    std::string result = "";
    for(uint i=0;i<content.size();++i){
        result += open_bracket_if_necessary(priority(),content[i]->priority());
        result += content[i]->to_rbg(indent);
        result += close_bracket_if_necessary(priority(),content[i]->priority());
    }
    return result;
}

std::string concatenation::to_rbg()const{
    std::string result = "";
    for(uint i=0;i<content.size();++i){
        result += open_bracket_if_necessary(priority(),content[i]->priority());
        result += content[i]->to_rbg();
        result += close_bracket_if_necessary(priority(),content[i]->priority());
    }
    return result;
}

std::unique_ptr<game_move> concatenation::flatten(void){
    std::vector<std::unique_ptr<game_move>> result;
    std::vector<std::unique_ptr<game_move>> first_modifier_block;
    for(auto& el: content)
        el->gather_concatenation_elements(result,first_modifier_block);
    if(!first_modifier_block.empty()){
        if(first_modifier_block.size()>1)
            result.push_back(std::unique_ptr<game_move>(new modifier_block(std::move(first_modifier_block))));
        else
            result.push_back(std::move(first_modifier_block[0]));
    }
    if(result.size()==1) // probably only modifiers
        return std::move(result[0]);
    else
        return std::unique_ptr<game_move>(new concatenation(std::move(result)));
}

void concatenation::gather_concatenation_elements(
    std::vector<std::unique_ptr<game_move>>& elements,
    std::vector<std::unique_ptr<game_move>>& next_block_elements){
    for(auto& el: content)
        el->gather_concatenation_elements(elements,next_block_elements);
}

straightness_result concatenation::compute_k_straightness(void)const{
    auto current_max = standard_non_switch();
    for(const auto& el: content)
        current_max.concatenate_result(el->compute_k_straightness());
    return current_max;
}

const std::vector<std::unique_ptr<game_move>>& concatenation::get_content(void)const{
    return content;
}

void concatenation::add_move(std::unique_ptr<game_move> m){
    content.push_back(std::move(m));
}

}
