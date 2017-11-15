#ifndef ONS
#define ONS

#include<set>
#include<vector>

#include"token.hpp"
#include"pure_game_move.hpp"
#include"message.hpp"
#include"slice_iterator.hpp"
#include"parser_helpers.hpp"

class ons : public pure_game_move{
        std::set<token> legal_ons;
        ons(std::vector<token>&& legal_ons);
        ons(void);
    public:
        ~ons(void)override=default;
        friend parser_result<ons> parse_ons(slice_iterator& it, const std::set<token>& declared_pieces, messages_container& msg)throw(message);
        friend parser_result<ons>;
};

parser_result<ons> parse_ons(slice_iterator& it, const std::set<token>& declared_pieces, messages_container& msg)throw(message);

#endif
