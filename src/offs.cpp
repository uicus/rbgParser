#include"offs.hpp"
#include"sum.hpp"

namespace rbg_parser{

off::off(token&& off_piece):
off_piece(std::move(off_piece)){
}

int off::give_indices_in_expression(int next_free){
    index = next_free;
    return next_free+1;
}

void off::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

const token& off::get_piece(void)const{
    return off_piece;
}

std::string off::to_rbg(const options& opt, uint)const{
    return to_rbg(opt);
}

std::string off::to_rbg(const options& opt)const{
    std::string result = "";
    if(opt.enabled_noop_before_modifier())
        result += ".";
    result += "[";
    result += off_piece.to_string();
    result += "]";
    if(opt.enabled_noop_after_modifier())
        result += ".";
    return result;
}

std::unique_ptr<game_move> off::flatten(void){
    return std::unique_ptr<game_move>(new off(std::move(*this)));
}

straightness_result off::compute_k_straightness(void)const{
    return modifier_non_switch();
}

bool off::check_if_redundant(std::set<token>&, bool& already_met_off)const{
    if(already_met_off)
        return true;
    already_met_off = true;
    return false;
}

std::unique_ptr<game_move> make_offs_sum(std::vector<token>&& legal_offs){
    sum* result = new sum;
    for(auto& el: legal_offs)
        result->add_move(std::unique_ptr<game_move>(new off(std::move(el))));
    return std::unique_ptr<game_move>(result);
}

std::unique_ptr<game_move> off::copy(void)const{
    return std::unique_ptr<game_move>(new off(token(off_piece)));
}

bool off::is_modifier(void)const{
    return true;
}

}
