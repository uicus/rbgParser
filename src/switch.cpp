#include"switch.hpp"

player_switch::player_switch(token&& to_player):
to_player(std::move(to_player)),
turn_changer(true){
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
