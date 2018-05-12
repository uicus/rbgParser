#include"sloth.hpp"

namespace rbg_parser{

void sloth::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

std::string sloth::to_rbg(uint)const{
    return to_rbg();
}

std::string sloth::to_rbg()const{
    return "@";
}

std::unique_ptr<game_move> sloth::flatten(void){
    return std::unique_ptr<game_move>(new sloth(std::move(*this)));
}

}
