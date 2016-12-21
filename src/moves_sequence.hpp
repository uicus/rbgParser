#ifndef MOVES_SEQUENCE
#define MOVES_SEQUENCE

#include<vector>
#include<ostream>

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
        friend std::ostream& operator<<(std::ostream& out,const moves_sequence& m);
};

moves_sequence parse_moves_sequence(
    slice_iterator& it,
    std::set<token>& encountered_pieces,
    const game_order& players,
    uint player_number,
    messages_container& msg)throw(message);
std::ostream& operator<<(std::ostream& out,const moves_sequence& m);

#endif
