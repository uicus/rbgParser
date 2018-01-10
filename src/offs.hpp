#ifndef OFFS
#define OFFS

#include<cassert>

#include"game_move.hpp"
#include"token.hpp"
#include"sum.hpp"
#include"message.hpp"
#include"slice_iterator.hpp"
#include"parser_helpers.hpp"
#include"declarations.hpp"
#include"abstract_dispatcher.hpp"

namespace rbg_parser{

class off : public game_move{
        token off_piece;
        bool lazy;
        off(token&& off_piece, bool lazy=false);
        off(void)=default;
    public:
        ~off(void)override=default;
        off(off&&)=default;
        off(const off&)=default;
        off& operator=(const off&)=default;
        off& operator=(off&&)=default;
        void set_lazy(void)override{lazy=true;};
        std::unique_ptr<pure_game_move> transform_into_pure(void)override{assert(false);};
        std::unique_ptr<game_move> simplify(void)override{return std::unique_ptr<game_move>(new off(std::move(*this)));};
        void accept(abstract_dispatcher& dispatcher)const override;
        uint priority(void)const override{return 3;};
        std::string to_rbg(uint)const override;
        std::string to_rbg()const override;
        std::unique_ptr<game_move> flatten(void)override;
        void gather_concatenation_elements(
            std::vector<std::unique_ptr<game_move>>& elements,
            std::vector<std::unique_ptr<game_move>>& next_block_elements)override;
        straightness_result compute_k_straightness(void)const override;
        bool check_if_redundant(std::set<token>&, bool& already_met_off)const override;
        const token& get_piece(void)const;
        bool is_lazy(void)const;
        friend parser_result<sum> parse_offs(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);
};

parser_result<sum> parse_offs(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);

}

#endif
