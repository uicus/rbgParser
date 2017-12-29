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

const game_board& parsed_game::get_board(void)const{
    return brd;
}

const game_move* parsed_game::get_moves(void)const{
    return moves.get();
}

std::string parsed_game::to_rbg(bool pretty)const{
    return "#game = "+name+"\n"
          +brd.to_rbg(pretty)
          +decl.to_rbg()
          +"#rules = "+(pretty ? "\n"+moves->to_rbg(1) : moves->to_rbg())+"\n";
}
