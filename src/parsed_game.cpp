#include"parsed_game.hpp"
#include"gdl_constants.hpp"

parsed_game::parsed_game(
    std::string&& n,
    game_board&& b,
    std::map<token,moves_sequence>&& m,
    std::map<token,goals_alternative>&& g,
    game_order&& pl,
    std::set<token>&& kp)noexcept:
name(std::move(n)),
brd(std::move(b)),
players(std::move(pl)),
moves(std::move(m)),
goals(std::move(g)),
known_pieces(std::move(kp)){}

void parsed_game::to_simple(const options& o){
    uint current_id = 0;
    for(auto& el:moves){
        std::set<token> splitters;
        el.second.flatten();
        el.second.prepare_to_split(known_pieces,splitters,current_id,o);
        el.second.split_into_semisteps(splitters);
    }
}

std::ostream& operator<<(std::ostream& out,const parsed_game& g){
    out<<"#game \""<<g.name<<"\"\n\n";
    out<<g.brd<<'\n';
    out<<g.players<<'\n';
    for(const auto& el: g.moves)
        out<<"#player "<<el.first.to_string()<<'\n'<<el.second<<'\n';
    for(const auto& el: g.goals)
        out<<"#goal "<<el.first.to_string()<<'\n'<<el.second<<'\n';
    return out;
}

void parsed_game::print_roles(std::ostream& out,const options& o)const{
    if(o.printing_comments())
        out<<section_title("Roles")<<'\n';
    for(uint i=0;i<players.get_number_of_players();++i)
        out<<"(role "<<players.get_player_name(i).to_string()<<")\n";
}

void parsed_game::to_gdl(std::ostream& out,const options& o)const{
    if(o.printing_comments())
        out<<section_title(std::string(name))<<'\n';
    print_roles(out,o);
}
