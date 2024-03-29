#include"declarations.hpp"
#include"printer_helpers.hpp"

namespace rbg_parser{

declarations::declarations(
    std::vector<std::pair<token, uint>>&& players_names_,
    std::set<token>&& pieces_names,
    std::map<token, uint>&& variables_names):
players_names(),
ordered_player_names(std::move(players_names_)),
pieces_names(std::move(pieces_names)),
variables_names(std::move(variables_names)),
edges_names(){
    for(const auto& el: ordered_player_names)
        players_names.insert(el);
}

const std::set<token>& declarations::get_legal_pieces(void)const{
    return pieces_names;
}

const std::map<token, uint>& declarations::get_legal_players(void)const{
    return players_names;
}

const std::vector<std::pair<token, uint>>& declarations::get_legal_players_ordered(void)const{
    return ordered_player_names;
}

const std::map<token, uint>& declarations::get_legal_variables(void)const{
    return variables_names;
}

const std::set<token>& declarations::get_legal_edges(void)const{
    return edges_names;
}

uint declarations::get_player_bound(const token& player_name)const{
    return players_names.at(player_name);
}

uint declarations::get_variable_bound(const token& variable_name)const{
    return variables_names.at(variable_name);
}

void declarations::add_edge_label(const token& name){
    edges_names.insert(name);
}

std::string declarations::to_rbg(void)const{
    return "#players = "+rbg_parser::to_rbg(ordered_player_names)+"\n"
          +"#pieces = "+rbg_parser::to_rbg(pieces_names)+"\n"
          +"#variables = "+rbg_parser::to_rbg(variables_names)+"\n";
}

}
