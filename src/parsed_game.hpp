#ifndef PARSED_GAME
#define PARSED_GAME

#include"game_board.hpp"

class parsed_game{
        game_board brd;
    public:
        parsed_game(game_board&& brd);
};

#endif
