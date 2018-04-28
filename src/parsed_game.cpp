#include"parsed_game.hpp"
namespace rbg_parser{

parsed_game::parsed_game(
std::string&& name,
declarations&& decl,
game_board&& brd,
graph&& g,
std::unique_ptr<game_move> moves,
uint bound_val):
name(std::move(name)),
decl(std::move(decl)),
brd(std::move(brd)),
g(std::move(g)),
moves(std::move(moves)),
bound_val(bound_val),
straightness(){
    straightness = this->moves->compute_k_straightness().final_result();
}

const declarations& parsed_game::get_declarations(void)const{
    return decl;
}

const game_board& parsed_game::get_board(void)const{
    return brd;
}

const game_move* parsed_game::get_moves(void)const{
    return moves.get();
}

const std::string& parsed_game::get_name(void)const{
    return name;
}

uint parsed_game::get_bound(void)const{
    return bound_val;
}

int parsed_game::get_straightness(void)const{
    return straightness;
}

std::string parsed_game::to_rbg(bool pretty)const{
    return "#game = "+name+"\n"
          +brd.to_rbg(pretty)
//          +"#board ="+g.to_rbg(pretty)
          +decl.to_rbg()
          +"#rules = "+(pretty ? "\n"+moves->to_rbg(1) : moves->to_rbg())+"\n"
          +"#bound = "+std::to_string(bound_val)+"\n";
}

}
