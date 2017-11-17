#ifndef BRACKETED_MOVE
#define BRACKETED_MOVE

#include<memory>

#include"game_move.hpp"
#include"types.hpp"
#include"message.hpp"
#include"slice_iterator.hpp"
#include"parser_helpers.hpp"
#include"declarations.hpp"

class bracketed_move : public game_move{
        std::unique_ptr<game_move> contained_move;
        uint number_of_repetitions;
        bracketed_move(void);
        bracketed_move(std::unique_ptr<game_move> contained_move, uint number_of_repetitions=1);
    public:
        ~bracketed_move(void)override=default;
        bracketed_move(bracketed_move&&)=default;
        bool modifies(void)override;
        friend parser_result<bracketed_move> parse_bracketed_move(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);
        friend parser_result<bracketed_move>;
};

parser_result<bracketed_move> parse_bracketed_move(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);

#endif
