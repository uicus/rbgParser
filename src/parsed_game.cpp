#include"parsed_game.hpp"

parsed_game::parsed_game(game_board&& b):
brd(std::move(b)){
}
