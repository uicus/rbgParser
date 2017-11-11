#ifndef PARSED_GAME
#define PARSED_GAME

#include<string>

#include"declarations.hpp"
#include"game_board.hpp"

class parsed_game{
        std::string name;
        declarations decl;
        game_board brd;
    public:
        parsed_game(
            std::string&& name,
            declarations&& decl,
            game_board&& brd);
};

#endif
