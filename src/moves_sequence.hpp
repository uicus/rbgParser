#ifndef MOVES_SEQUENCE
#define MOVES_SEQUENCE

#include<vector>

#include"game_move.hpp"
#include"slice_iterator.hpp"
#include"game_order.hpp"
#include"message.hpp"
#include"parser_helpers.hpp"

class moves_sequence{
        std::vector<moves_sum> sequence;
    public:
        moves_sequence(void);

        friend moves_sequence parse_moves_sequence(
            slice_iterator& it,
            std::set<token>& encountered_pieces,
            const game_order& players,
            uint player_number,
            messages_container& msg)throw(message);
};

moves_sequence parse_moves_sequence(
    slice_iterator& it,
    std::set<token>& encountered_pieces,
    const game_order& players,
    uint player_number,
    messages_container& msg)throw(message);

#endif
