#ifndef GAME_ORDER
#define GAME_ORDER

#include<set>
#include<vector>

#include"token.hpp"
#include"parser_helpers.hpp"
#include"message.hpp"
#include"slice.hpp"
#include"slice_iterator.hpp"

class game_order{
        std::set<token> players_set;
        std::vector<token> players_order;
    public:
        game_order(void);

        friend parser_result<game_order> parse_game_order(
            slice_iterator& it,
            messages_container& msg,
            const std::map<token,slice>& players)throw(message);

        const token& get_player_name(uint my_number,int delta)const;
        bool exists(const token& player_name)const;
};

parser_result<game_order> parse_game_order(
    slice_iterator& it,
    messages_container& msg,
    const std::map<token,slice>& players)throw(message);

#endif
