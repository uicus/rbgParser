#ifndef SWITCH
#define SWITCH

#include<cassert>

#include"game_move.hpp"
#include"message.hpp"
#include"slice_iterator.hpp"
#include"parser_helpers.hpp"
#include"declarations.hpp"

class player_switch : public game_move{
        token to_player;
        bool turn_changer;
        player_switch(token&& to_player);
    public:
        ~player_switch(void)override=default;
        player_switch(void)=default;
        player_switch(player_switch&&)=default;
        player_switch(const player_switch&)=default;
        player_switch& operator=(const player_switch&)=default;
        player_switch& operator=(player_switch&&)=default;
        std::unique_ptr<pure_game_move> transform_into_pure(void)override{assert(false);};
        friend parser_result<player_switch> parse_player_switch(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);
        friend parser_result<player_switch>;
};

parser_result<player_switch> parse_player_switch(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);

#endif
