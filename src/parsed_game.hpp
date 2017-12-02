#ifndef PARSED_GAME
#define PARSED_GAME

#include<string>
#include<memory>

#include"declarations.hpp"
#include"game_board.hpp"
#include"game_move.hpp"

class parsed_game{
        std::string name;
        declarations decl;
        game_board brd;
        std::unique_ptr<game_move> moves;
    public:
        parsed_game(
            std::string&& name,
            declarations&& decl,
            game_board&& brd,
            std::unique_ptr<game_move> moves);
};

#endif
