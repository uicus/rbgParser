#ifndef SHIFT
#define SHIFT

#include"pure_game_move.hpp"
#include"message.hpp"
#include"slice_iterator.hpp"
#include"parser_helpers.hpp"

class shift : public pure_game_move{
        int x;
        int y;
        shift(int x,int y);
        shift(void);
    public:
        ~shift(void)override=default;
        friend parser_result<shift> parse_shift(slice_iterator& it, messages_container& msg)throw(message);
        friend parser_result<shift>;
};

parser_result<shift> parse_shift(slice_iterator& it, messages_container& msg)throw(message);

#endif
