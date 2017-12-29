#include"declarations.hpp"
#include"printer_helpers.hpp"

declarations::declarations(std::set<token>&& players_names, std::set<token>&& pieces_names, std::set<token>&& variables_names):
players_names(std::move(players_names)),
pieces_names(std::move(pieces_names)),
variables_names(std::move(variables_names)){
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

std::string declarations::to_rbg(void)const{
    return "#players = "+::to_rbg(players_names)+"\n"
          +"#pieces = "+::to_rbg(pieces_names)+"\n"
          +"#variables = "+::to_rbg(variables_names)+"\n";
}
