#include"moves_sequence.hpp"

moves_sequence::moves_sequence(void):
sequence(){}

moves_sequence parse_moves_sequence(
    slice_iterator& it,
    std::set<token>& encountered_pieces,
    const game_order& players,
    uint player_number,
    messages_container& msg)throw(message){
    if(!it.has_value())
        throw msg.build_message("Empty moves sequence for player \'"+players.get_player_name(player_number,0).to_string()+"\'");
    bool contains_turn_changer = false;
    moves_sequence result;
    bool this_move_contains_turn_changer = false;
    parser_result<moves_sum> sum_result = parse_moves_sum(it,encountered_pieces,players,player_number,this_move_contains_turn_changer,msg);
    contains_turn_changer |= this_move_contains_turn_changer;
    if(sum_result.is_success())
        result.sequence.push_back(sum_result.move_value());
    else
        throw msg.build_message(it.create_call_stack("Expected moves sum, encountered \'"+it.current().to_string()+"\'"));
    while(it.has_value() && it.current().get_type() == double_plus){
        if(!it.next(msg))
            throw msg.build_message("Unexpected end of moves sequence (player \'"+players.get_player_name(player_number,0).to_string()+"\')");
        this_move_contains_turn_changer = false;
        sum_result = parse_moves_sum(it,encountered_pieces,players,player_number,this_move_contains_turn_changer,msg);
        contains_turn_changer |= this_move_contains_turn_changer;
        if(!sum_result.is_success())
            throw msg.build_message(it.create_call_stack("Expected moves sum, encountered \'"+it.current().to_string()+"\'"));
        else
            result.sequence.push_back(sum_result.move_value());
    }
    if(!contains_turn_changer)
        msg.add_message("Player \'"+players.get_player_name(player_number,0).to_string()+"\' has no moves which change the turn");
    if(it.has_value())
        throw msg.build_message(it.create_call_stack("Unexpected tokens at the end of \'"+players.get_player_name(player_number,0).to_string()+"\' \'player\' segment"));
    return result;
}
