#include"parsed_game.hpp"

parsed_game::parsed_game(
std::string&& name,
declarations&& decl,
game_board&& brd):
name(std::move(name)),
decl(std::move(decl)),
brd(std::move(brd)){
}
