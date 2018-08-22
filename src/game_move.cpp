#include"game_move.hpp"
#include"modifier_block.hpp"

namespace rbg_parser{

void game_move::gather_concatenation_elements(
    std::vector<std::unique_ptr<game_move>>& elements,
    std::vector<std::unique_ptr<game_move>>& next_block_elements){
    if(!next_block_elements.empty()){
        if(next_block_elements.size()>1)
            elements.push_back(std::unique_ptr<game_move>(new modifier_block(std::move(next_block_elements))));
        else
            elements.push_back(std::move(next_block_elements[0]));
        next_block_elements.clear();
    }
    elements.push_back(flatten());
}

}
