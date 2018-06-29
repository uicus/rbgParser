#include"shift.hpp"

namespace rbg_parser{

shift::shift(const token& edge_name):
edge_name(edge_name){
}

void shift::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

std::string shift::to_rbg(uint)const{
    return to_rbg();
}

std::unique_ptr<game_move> shift::flatten(void){
    return std::unique_ptr<game_move>(new shift(std::move(*this)));
}

std::string shift::to_rbg()const{
    std::string result = "";
    result += edge_name.to_string();
    result += " ";
    return result;
}

void shift::gather_concatenation_elements(
    std::vector<std::unique_ptr<game_move>>&,
    std::vector<std::unique_ptr<game_move>>& next_block_elements){
    next_block_elements.push_back(flatten());
}

const token& shift::get_content(void)const{
    return edge_name;
}

}
