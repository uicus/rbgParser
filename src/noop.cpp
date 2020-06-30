#include"noop.hpp"
#include"printer_helpers.hpp"

namespace rbg_parser{

int noop::give_indices_in_expression(int next_free){
    index = next_free;
    return next_free+1;
}

void noop::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

std::string noop::to_rbg(uint)const{
    return to_rbg();
}

std::string noop::to_rbg()const{
    return ".";
}

std::unique_ptr<game_move> noop::flatten(void){
    return std::unique_ptr<game_move>(new noop);
}

std::unique_ptr<game_move> noop::copy(void)const{
    return std::unique_ptr<game_move>(new noop);
}

}
