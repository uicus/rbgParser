#include"parsed_game.hpp"
namespace rbg_parser{

parsed_game::parsed_game(
declarations&& decl,
graph&& g,
std::unique_ptr<game_move> moves):
decl(std::move(decl)),
g(std::move(g)),
moves(std::move(moves)) {}

const declarations& parsed_game::get_declarations(void)const{
    return decl;
}

const graph& parsed_game::get_board(void)const{
    return g;
}

const game_move* parsed_game::get_moves(void)const{
    return moves.get();
}

int parsed_game::get_straightness(StraightnessType st)const{
    return this->moves->compute_k_straightness(st).final_result(st);
}

std::string parsed_game::to_rbg(const options& opt, bool pretty)const{
    return "#board ="+g.to_rbg(pretty)
          +decl.to_rbg()
          +"#rules = "+(pretty ? "\n"+moves->to_rbg(opt, 1) : moves->to_rbg(opt))+"\n";
}

}
