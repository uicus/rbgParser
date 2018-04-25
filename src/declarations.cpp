#include"declarations.hpp"
#include"printer_helpers.hpp"

namespace rbg_parser{

declarations::declarations(std::set<token>&& players_names, std::set<token>&& pieces_names, std::set<token>&& variables_names, token&& first_player):
players_names(std::move(players_names)),
pieces_names(std::move(pieces_names)),
variables_names(std::move(variables_names)),
edges_names(),
first_player(std::move(first_player)){
}

const std::set<token>& declarations::get_legal_pieces(void)const{
    return pieces_names;
}

const std::set<token>& declarations::get_legal_players(void)const{
    return players_names;
}

const std::set<token>& declarations::get_legal_variables(void)const{
    return variables_names;
}

const std::set<token>& declarations::get_legal_edges(void)const{
    return edges_names;
}

void declarations::add_edge_label(const token& name){
    edges_names.insert(name);
}

const token& declarations::get_first_player(void)const{
    return first_player;
}

std::string declarations::to_rbg(void)const{
    return "#players = "+first_player.to_string()+","+rbg_parser::to_rbg_without(players_names,first_player)+"\n"
          +"#pieces = "+rbg_parser::to_rbg(pieces_names)+"\n"
          +"#variables = "+rbg_parser::to_rbg(variables_names)+"\n";
}

}
