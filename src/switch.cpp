#include"switch.hpp"

namespace rbg_parser{

player_switch::player_switch(token&& to_player):
to_player(std::move(to_player)),
turn_changer(true){
}

void player_switch::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

std::string player_switch::to_rbg(uint)const{
    return to_rbg();
}

std::string player_switch::to_rbg()const{
    std::string result = "->";
    if(changes_player())
        result += to_player.to_string();
    return result;
}

std::unique_ptr<game_move> player_switch::flatten(void){
    return std::unique_ptr<game_move>(new player_switch(std::move(*this)));
}

const token& player_switch::get_player(void)const{
    return to_player;
}

bool player_switch::changes_player(void)const{
    return turn_changer;
}

parser_result<player_switch> parse_player_switch(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message){
    parsing_context_string_guard g(&it, "Unexpected end of input while parsing switch");
    if(!it.has_value() || it.current(msg).get_type() != arrow)
        return failure<player_switch>();
    it.next(msg);
    if(it.current(msg).get_type() != identifier)
        return success(player_switch());
    token name = it.current(msg);
    if(decls.get_legal_players().count(name) == 0)
        throw msg.build_message(it.create_call_stack("Player \'"+it.current(msg).to_string()+"\' was not declared in respective segment"));
    it.next(msg);
    return success(player_switch(std::move(name)));
}

}
