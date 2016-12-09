#include"parsed_game.hpp"

parsed_game::parsed_game(game_board&& b,std::map<token,moves_sequence>&& m,game_order&& pl):
brd(std::move(b)),
players(std::move(pl)),
moves(std::move(m)){
}
