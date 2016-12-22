#include"game_order.hpp"

#include<cassert>

game_order::game_order(void)noexcept:
players_set(),
players_order(){}

game_order parse_game_order(
    slice_iterator& it,
    messages_container& msg,
    const std::map<token,slice>& players)throw(message){
    game_order result;
    while(it.has_value() && it.current().get_type() == identifier){
        if(players.count(it.current())==0)
            msg.add_message(it.create_call_stack("Player \'"+it.current().to_string()+"\' found in the \'order\' directive but not in the \'player\' and \'goal\', ignoring this player"));
        else{
            result.players_order.push_back(it.current());
            result.players_set.insert(it.current());
        }
        it.next(msg);
    }
    if(players.size()>result.players_set.size())
        msg.add_message("There are players not listed in the \'order\' directive but defined in the \'player\' and \'goal\', ignoring them");
    if(it.has_value())
        throw msg.build_message(it.create_call_stack("Unexpected tokens at the end of \'order\' segment"));
    return result;
}

const token& game_order::get_player_name(uint my_number,int delta)const{
    assert(my_number<players_order.size());
    if(delta>=0)return players_order[(my_number+delta)%players_order.size()];
    else return players_order[((my_number+players_order.size())-((-delta)%players_order.size()))%players_order.size()];
}

bool game_order::exists(const token& player_name)const{
    return players_set.count(player_name)>0;
}

uint game_order::get_number_of_players(void)const{
    return players_order.size();
}

std::ostream& operator<<(std::ostream& out,const game_order& g){
    out<<"#order\n";
    for(uint i=0;i<g.get_number_of_players();++i)
        out<<"    "<<g.players_order[i].to_string()<<'\n';
    return out;
}
