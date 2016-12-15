#ifndef PARSED_GAME
#define PARSED_GAME

#include<map>

#include"game_board.hpp"
#include"token.hpp"
#include"moves_sequence.hpp"
#include"game_goal.hpp"

class parsed_game{
        game_board brd;
        game_order players;
        std::map<token,moves_sequence> moves;
        std::map<token,goals_alternative> goals;
    public:
        parsed_game(game_board&& brd,std::map<token,moves_sequence>&& m,std::map<token,goals_alternative>&& g,game_order&& pl);
};

#endif
