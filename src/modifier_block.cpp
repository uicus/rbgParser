#include"modifier_block.hpp"
#include"printer_helpers.hpp"

namespace rbg_parser{

modifier_block::modifier_block(std::vector<std::unique_ptr<game_move>>&& content):
concatenation(std::move(content)){
    remove_redundant_modifiers();
}

void modifier_block::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

straightness_result modifier_block::compute_k_straightness(void)const{
    return modifier_non_switch();
}

void modifier_block::remove_redundant_modifiers(void){
    std::vector<bool> slated_for_removal(content.size(),false);
    std::set<token> met_assignments;
    bool met_off=false;
    for(uint i=content.size();i>0;--i)
        if(content[i-1]->check_if_redundant(met_assignments,met_off))
            slated_for_removal[i-1] = true;
    std::vector<std::unique_ptr<game_move>> result;
    for(uint i=0;i<content.size();++i)
        if(!slated_for_removal[i])
            result.push_back(std::move(content[i]));
    std::swap(result,content);
}

}
