#include"modifier_block.hpp"
#include"printer_helpers.hpp"

namespace rbg_parser{

modifier_block::modifier_block(std::vector<std::unique_ptr<game_move>>&& content):
concatenation(std::move(content)){
}

void modifier_block::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

straightness_result modifier_block::compute_k_straightness(void)const{
    return modifier_non_switch();
}

}
