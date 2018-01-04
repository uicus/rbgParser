#ifndef PARSED_GAME
#define PARSED_GAME

#include<string>
#include<memory>

#include"declarations.hpp"
#include"game_board.hpp"
#include"game_move.hpp"

namespace rbg_parser{

class parsed_game{
        std::string name;
        declarations decl;
        game_board brd;
        std::unique_ptr<game_move> moves;
        int straightness;
    public:
        parsed_game(
            std::string&& name,
            declarations&& decl,
            game_board&& brd,
            std::unique_ptr<game_move> moves);
        const declarations& get_declarations(void)const;
        const game_board& get_board(void)const;
        const game_move* get_moves(void)const;
        int get_straightness(void)const;
        std::string to_rbg(bool pretty=false)const;
};

}

#endif
