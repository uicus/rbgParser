#ifndef GAME_MOVE
#define GAME_MOVE

#include<set>

#include"token.hpp"
#include"parser_helpers.hpp"
#include"slice_iterator.hpp"
#include"message.hpp"

class atomic_move{
        int x;
        int y;
        bool every_on_legal;
        std::set<token> on;
        std::set<token> off;
    public:
        atomic_move(void);
        atomic_move(int x,int y,const std::set<token>& on,const std::set<token>& off);
        atomic_move(int x,int y,const std::set<token>& off); // true -> on, false -> off

        friend parser_result<atomic_move> parse_atomic_move(slice_iterator& it, messages_container& msg)throw(message);
};

parser_result<atomic_move> parse_atomic_move(slice_iterator& it, messages_container& msg)throw(message);

#endif
