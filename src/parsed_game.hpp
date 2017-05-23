#ifndef PARSED_GAME
#define PARSED_GAME

#include<map>
#include<string>
#include<ostream>

#include"game_board.hpp"
#include"token.hpp"
#include"moves_sequence.hpp"
#include"game_goal.hpp"
#include"options.hpp"

class parsed_game{
        std::string name;
        game_board brd;
        game_order players;
        std::map<token,moves_sequence> moves;
        std::map<token,goals_alternative> goals;
        std::set<token> known_pieces;
        void print_pieces(std::ostream& out,const options& o)const;
        void print_base(std::ostream& out,const options& o,int max_turn)const;
        void print_input(std::ostream& out,const options& o)const;
        void print_initial_state(std::ostream& out,const options& o,int max_turn)const;
        void print_turn_counter(std::ostream& out,const options& o,int max_turn, int max_equivalency)const;
        void print_moves(std::ostream& out,const options& o,uint& next_free_id)const;
        void print_arithmetics(std::ostream& out,const options& o,int max_n)const;
        void print_next_state(std::ostream& out,const options& o,int max_turn)const;
    public:
        parsed_game(
            std::string&& name,
            game_board&& brd,
            std::map<token,moves_sequence>&& m,
            std::map<token,goals_alternative>&& g,
            game_order&& pl,
            std::set<token>&& known_pieces)noexcept;

        friend std::ostream& operator<<(std::ostream& out,const parsed_game& g);
        void to_simple(const options& o);
        void to_gdl(std::ostream& out,const options& o)const;
};

std::ostream& operator<<(std::ostream& out,const parsed_game& g);

#endif
