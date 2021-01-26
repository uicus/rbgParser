#ifndef OFFS
#define OFFS

#include<cassert>

#include"game_move.hpp"
#include"token.hpp"
#include"abstract_dispatcher.hpp"
#include"options.hpp"

namespace rbg_parser{

class off : public game_move{
        token off_piece;
        off(token&& off_piece);
        off(void)=default;
    public:
        ~off(void)override=default;
        off(off&&)=default;
        off(const off&)=delete;
        off& operator=(const off&)=delete;
        off& operator=(off&&)=default;
        int give_indices_in_expression(int next_free)override;
        std::unique_ptr<game_move> simplify(void)override{return std::unique_ptr<game_move>(new off(std::move(*this)));};
        void accept(abstract_dispatcher& dispatcher)const override;
        uint priority(const options& opt)const override{return (opt.enabled_noop_after_modifier() or opt.enabled_noop_before_modifier()) ? 2 : 4;};
        std::string to_rbg(const options& opt, uint)const override;
        std::string to_rbg(const options&)const override;
        std::unique_ptr<game_move> flatten(void)override;
        straightness_result compute_k_straightness(StraightnessType st)const override;
        bool check_if_redundant(std::set<token>&, bool& already_met_off)const override;
        const token& get_piece(void)const;
        bool is_lazy(void)const;
        std::unique_ptr<game_move> copy(void)const override;
        bool is_modifier(void)const override;
        friend std::unique_ptr<game_move> make_offs_sum(std::vector<token>&& legal_offs);
};

std::unique_ptr<game_move> make_offs_sum(std::vector<token>&& legal_offs);

}

#endif
