#include"switch.hpp"
#include"modifier_block.hpp"

namespace rbg_parser{

player_switch::player_switch(token&& to_player):
to_player(std::move(to_player)),
non_deterministic_keeper(false),
deterministic_keeper(false){
}

player_switch::player_switch(bool deterministic):
to_player(),
non_deterministic_keeper(!deterministic),
deterministic_keeper(deterministic){
}

void player_switch::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

std::string player_switch::to_rbg(uint)const{
    return to_rbg();
}

std::string player_switch::to_rbg()const{
    std::string result = "->";
    if(is_non_deterministic_keeper())
        result += '*';
    else if(!is_deterministic_keeper())
        result += to_player.to_string();
    return result;
}

std::unique_ptr<game_move> player_switch::flatten(void){
    return std::unique_ptr<game_move>(new player_switch(std::move(*this)));
}

void player_switch::gather_concatenation_elements(
    std::vector<std::unique_ptr<game_move>>& elements,
    std::vector<std::unique_ptr<game_move>>& next_block_elements){
    next_block_elements.push_back(flatten());
    if(next_block_elements.size()>1)
        elements.push_back(std::unique_ptr<game_move>(new modifier_block(std::move(next_block_elements))));
    else
        elements.push_back(std::move(next_block_elements[0]));
    next_block_elements.clear();
}

straightness_result player_switch::compute_k_straightness(void)const{
    return standard_switch();
}

bool player_switch::finalizer_elligible(void)const{
    return false;
}

const token& player_switch::get_player(void)const{
    return to_player;
}

bool player_switch::is_deterministic_keeper(void)const{
    return deterministic_keeper;
}
bool player_switch::is_non_deterministic_keeper(void)const{
    return non_deterministic_keeper;
}

parser_result<player_switch> parse_player_switch(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message){
    parsing_context_string_guard g(&it, "Unexpected end of input while parsing switch");
    if(!it.has_value() || it.current(msg).get_type() != arrow)
        return failure<player_switch>();
    it.next(msg);
    if(!it.has_value() || (it.current(msg).get_type() != identifier && it.current(msg).get_type() != star))
        return success(player_switch(true));
    token name = it.current(msg);
    if(name.get_type() != star && decls.get_legal_players().count(name) == 0)
        throw msg.build_message(it.create_call_stack("Player \'"+it.current(msg).to_string()+"\' was not declared in respective segment"));
    it.next(msg);
    if(name.get_type() == star)
        return success(player_switch(false));
    else
        return success(player_switch(std::move(name)));
}

}
