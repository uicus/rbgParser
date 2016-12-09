#ifndef PARSED_GAME
#define PARSED_GAME

#include<map>

#include"game_board.hpp"
#include"token.hpp"
#include"moves_sequence.hpp"

class parsed_game{
        game_board brd;
        game_order players;
        std::map<token,moves_sequence> moves;
    public:
        parsed_game(game_board&& brd,std::map<token,moves_sequence>&& m,game_order&& pl);
};

#endif
