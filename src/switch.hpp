#ifndef SWITCH
#define SWITCH

#include<cassert>

#include"game_move.hpp"
#include"message.hpp"
#include"slice_iterator.hpp"
#include"parser_helpers.hpp"
#include"declarations.hpp"
#include"abstract_dispatcher.hpp"

namespace rbg_parser{

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
        std::unique_ptr<game_move> simplify(void)override{return std::unique_ptr<game_move>(new player_switch(std::move(*this)));};
        void accept(abstract_dispatcher& dispatcher)const override;
        uint priority(void)const override{return 3;};
        std::string to_rbg(uint)const override;
        std::string to_rbg()const override;
        std::unique_ptr<game_move> flatten(void)override;
        void gather_concatenation_elements(
            std::vector<std::unique_ptr<game_move>>& elements,
            std::vector<std::unique_ptr<game_move>>& next_block_elements)override;
        straightness_result compute_k_straightness(void)const override;
        bool has_finisher(void)const override{return true;};
        const token& get_player(void)const;
        bool changes_player(void)const;
        friend parser_result<player_switch> parse_player_switch(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);
        friend parser_result<player_switch>;
};

parser_result<player_switch> parse_player_switch(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);

}

#endif
