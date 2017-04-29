#ifndef MOVES_SEQUENCE
#define MOVES_SEQUENCE

#include<vector>
#include<ostream>
#include<set>

#include"game_move.hpp"
#include"slice_iterator.hpp"
#include"game_order.hpp"
#include"message.hpp"
#include"parser_helpers.hpp"
#include"options.hpp"

class moves_sequence{
        std::vector<moves_sum> sequence;
    public:
        moves_sequence(void)noexcept;

        friend moves_sequence parse_moves_sequence(
            slice_iterator& it,
            std::set<token>& encountered_pieces,
            const game_order& players,
            uint player_number,
            messages_container& msg)throw(message);
        friend std::ostream& operator<<(std::ostream& out,const moves_sequence& m);
        void flatten(void);
        void prepare_to_split(std::set<token>& known_pieces,std::set<token>& pieces_after_split,uint& current_id,const options& o);
        void split_into_semisteps(const std::set<token>& splitters);

        void write_as_gdl(
            std::ostream& out,
            good_pieces_sets& s,
            const std::string& name,
            uint& next_free_id,
            const options& o)const;
};

moves_sequence parse_moves_sequence(
    slice_iterator& it,
    std::set<token>& encountered_pieces,
    const game_order& players,
    uint player_number,
    messages_container& msg)throw(message);
std::ostream& operator<<(std::ostream& out,const moves_sequence& m);

#endif
