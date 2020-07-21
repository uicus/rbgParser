#include<cassert>

#include"star_move.hpp"
#include"printer_helpers.hpp"

namespace rbg_parser{

star_move::star_move(std::unique_ptr<game_move>&& contained_move):
contained_move(std::move(contained_move)){
}

int star_move::give_indices_in_expression(int next_free){
    next_free = contained_move->give_indices_in_expression(next_free);
    return next_free;
}

bool star_move::modifies(void)const{
    if(contained_move)
        return contained_move->modifies();
    else
        return false;
}

std::unique_ptr<game_move> star_move::simplify(void){
    std::unique_ptr<game_move> content = contained_move->simplify();
    return std::unique_ptr<game_move>(new star_move(std::move(content)));
}

void star_move::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

std::string star_move::to_rbg(const options& opt, uint indent)const{
    std::string result = "";
    result += open_bracket_if_necessary(priority(opt),contained_move->priority(opt));
    result += contained_move->to_rbg(opt, indent);
    result += close_bracket_if_necessary(priority(opt),contained_move->priority(opt));
    result += "*";
    return result;
}

std::string star_move::to_rbg(const options& opt)const{
    std::string result = "";
    result += open_bracket_if_necessary(priority(opt),contained_move->priority(opt));
    result += contained_move->to_rbg(opt);
    result += close_bracket_if_necessary(priority(opt),contained_move->priority(opt));
    result += "*";
    return result;
}

std::unique_ptr<game_move> star_move::flatten(void){
    return std::unique_ptr<game_move>(new star_move(contained_move->flatten()));
}

straightness_result star_move::compute_k_straightness(void)const{
    auto content_result = contained_move->compute_k_straightness();
    content_result.repeat_result();
    return content_result;
}

const game_move* star_move::get_content(void)const{
    return contained_move.get();
}

std::unique_ptr<game_move> star_move::copy(void)const{
    return std::unique_ptr<game_move>(new star_move(contained_move->copy()));
}

bool star_move::has_piece_as_variable(const std::set<token>& pieces)const{
    return contained_move->has_piece_as_variable(pieces);
}

}
