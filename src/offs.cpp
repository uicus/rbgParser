#include"offs.hpp"
#include"sum.hpp"

namespace rbg_parser{

off::off(token&& off_piece, bool lazy):
off_piece(std::move(off_piece)),
lazy(lazy){
}

void off::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

const token& off::get_piece(void)const{
    return off_piece;
}

bool off::is_lazy(void)const{
    return lazy;
}

std::string off::to_rbg(uint)const{
    return to_rbg();
}

std::string off::to_rbg()const{
    std::string result = "";
    result += (lazy ? "[@" : "[");
    result += off_piece.to_string();
    result += "]";
    return result;
}

std::unique_ptr<game_move> off::flatten(void){
    return std::unique_ptr<game_move>(new off(std::move(*this)));
}

void off::gather_concatenation_elements(
    std::vector<std::unique_ptr<game_move>>&,
    std::vector<std::unique_ptr<game_move>>& next_block_elements){
    next_block_elements.push_back(flatten());
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

bool off::finalizer_elligible(void)const{
    return true;
}

std::unique_ptr<game_move> make_offs_sum(std::vector<token>&& legal_offs){
    sum* result = new sum;
    for(auto& el: legal_offs)
        result->add_move(std::unique_ptr<game_move>(new off(std::move(el))));
    return std::unique_ptr<game_move>(result);
}

}
