#include"sum.hpp"
#include"printer_helpers.hpp"

namespace rbg_parser{

sum::sum(std::vector<std::unique_ptr<game_move>>&& content):
content(std::move(content)){
}

int sum::give_indices_in_expression(int next_free){
    for(const auto& el: content)
        next_free = el->give_indices_in_expression(next_free);
    return next_free;
}

bool sum::modifies(void)const{
    for(const auto& el: content)
        if(el->modifies())
            return true;
    return false;
}

std::unique_ptr<game_move> sum::simplify(void){
    std::vector<std::unique_ptr<game_move>> result;
    for(uint i=0;i<content.size();++i)
        result.push_back(content[i]->simplify());
    if(result.size()==1)
        return std::move(result[0]);
    else
        return std::unique_ptr<game_move>(new sum(std::move(result)));
}

void sum::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

std::string sum::to_rbg(const options& opt, uint indent)const{
    std::string result = "";
    if(opt.enabled_noop_before_alternative())
        result += ".(";
    for(uint i=0;i<content.size();++i){
        result += (i==0 ? "\n":"\n+ ");
        if(opt.enabled_noop_after_alternative())
            result += ".";
        result += open_bracket_if_necessary(opt.enabled_noop_after_alternative() ? 2 : 1,content[i]->priority(opt));
        result += content[i]->to_rbg(opt, indent+1);
        result += close_bracket_if_necessary(opt.enabled_noop_after_alternative() ? 2 : 1,content[i]->priority(opt));
    }
    result += "\n";//+print_tabs(indent);
    if(opt.enabled_noop_before_alternative())
        result += ")";
    return result;
}

std::string sum::to_rbg(const options& opt)const{
    std::string result = "";
    if(opt.enabled_noop_before_alternative())
        result += ".(";
    for(uint i=0;i<content.size();++i){
        result += (i==0 ? "" : " + ");
        if(opt.enabled_noop_after_alternative())
            result += ".";
        result += open_bracket_if_necessary(opt.enabled_noop_after_alternative() ? 2 : 1,content[i]->priority(opt));
        result += content[i]->to_rbg(opt);
        result += close_bracket_if_necessary(opt.enabled_noop_after_alternative() ? 2 : 1,content[i]->priority(opt));
    }
    if(opt.enabled_noop_before_alternative())
        result += ")";
    return result;
}

std::unique_ptr<game_move> sum::flatten(void){
    std::vector<std::unique_ptr<game_move>> result;
    for(auto& el: content)
        el->gather_sum_elements(result);
    return std::unique_ptr<game_move>(new sum(std::move(result)));
}

void sum::gather_sum_elements(std::vector<std::unique_ptr<game_move>>& elements){
    for(auto& el: content)
        el->gather_sum_elements(elements);
}

straightness_result sum::compute_k_straightness(StraightnessType st)const{
    auto current_max = empty_move();
    for(const auto& el: content)
        current_max.max_of_results(el->compute_k_straightness(st));
    return current_max;
}

const std::vector<std::unique_ptr<game_move>>& sum::get_content(void)const{
    return content;
}

void sum::add_move(std::unique_ptr<game_move> m){
    content.push_back(std::move(m));
}

std::unique_ptr<game_move> sum::copy(void)const{
    std::vector<std::unique_ptr<game_move>> result_content;
    for(const auto& el: content)
        result_content.push_back(el->copy());
    return std::unique_ptr<game_move>(new sum(std::move(result_content)));
}

bool sum::has_piece_as_variable(const std::set<token>& pieces)const{
    for(const auto& el: content)
        if(el->has_piece_as_variable(pieces))
            return true;
    return false;
}

}
