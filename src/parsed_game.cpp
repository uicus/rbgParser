#include"parsed_game.hpp"

parsed_game::parsed_game(
std::string&& name,
declarations&& decl,
game_board&& brd,
std::unique_ptr<game_move> moves):
name(std::move(name)),
decl(std::move(decl)),
brd(std::move(brd)),
moves(std::move(moves)){
}
