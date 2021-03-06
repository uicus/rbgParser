#include"noop.hpp"
#include"printer_helpers.hpp"

namespace rbg_parser{

int noop::give_indices_in_expression(int next_free){
    index = next_free;
    return next_free;
}

void noop::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

std::string noop::to_rbg(const options& opt, uint)const{
    return to_rbg(opt);
}

std::string noop::to_rbg(const options&)const{
    return ".";
}

straightness_result noop::compute_k_straightness(StraightnessType st)const{
    if (st == StraightnessType::SEMIMOVE_STRAIGHTNESS) return standard_switch_beginning();
    return standard_non_switch();
}

std::unique_ptr<game_move> noop::flatten(void){
    return std::unique_ptr<game_move>(new noop);
}

std::unique_ptr<game_move> noop::copy(void)const{
    return std::unique_ptr<game_move>(new noop);
}

}
