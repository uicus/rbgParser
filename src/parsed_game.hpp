#ifndef PARSED_GAME
#define PARSED_GAME

#include<map>
#include<string>
#include<ostream>

#include"game_board.hpp"
#include"token.hpp"
#include"moves_sequence.hpp"
#include"game_goal.hpp"

class parsed_game{
        std::string name;
        game_board brd;
        game_order players;
        std::map<token,moves_sequence> moves;
        std::map<token,goals_alternative> goals;
        std::set<token> known_pieces;
    public:
        parsed_game(
            std::string&& name,
            game_board&& brd,
            std::map<token,moves_sequence>&& m,
            std::map<token,goals_alternative>&& g,
            game_order&& pl,
            std::set<token>&& known_pieces)noexcept;

        friend std::ostream& operator<<(std::ostream& out,const parsed_game& g);
        void to_simple(void);
};

std::ostream& operator<<(std::ostream& out,const parsed_game& g);

#endif
