#include"actions_block.hpp"
#include"printer_helpers.hpp"
#include"switch.hpp"

namespace rbg_parser{

actions_block::actions_block(std::vector<std::unique_ptr<game_move>>&& content):
concatenation(std::move(content)){
}

void actions_block::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

std::unique_ptr<game_move> actions_block::copy(void)const{
    std::vector<std::unique_ptr<game_move>> result_content;
    for(const auto& el: content)
        result_content.push_back(el->copy());
    return std::unique_ptr<game_move>(new concatenation(std::move(result_content)));
}

}
