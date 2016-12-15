#include"parsed_game.hpp"

parsed_game::parsed_game(game_board&& b,std::map<token,moves_sequence>&& m,std::map<token,goals_alternative>&& g,game_order&& pl):
brd(std::move(b)),
players(std::move(pl)),
moves(std::move(m)),
goals(std::move(g)){}
