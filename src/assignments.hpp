#ifndef ASSIGNMENTS
#define ASSIGNMENTS

#include<cassert>

#include"game_move.hpp"
#include"token.hpp"
#include"abstract_dispatcher.hpp"
#include"arithmetic_expression.hpp"
#include"options.hpp"
#include<memory>

namespace rbg_parser{

class assignment : public game_move{
        token left_side;
        std::unique_ptr<arithmetic_expression> right_side;
        assignment(void)=default;
    public:
        assignment(const token& left_side, std::unique_ptr<arithmetic_expression>&& right_side);
        ~assignment(void)override=default;
        assignment(assignment&&)=default;
        assignment(const assignment&)=delete;
        assignment& operator=(const assignment&)=delete;
        assignment& operator=(assignment&&)=default;
        int give_indices_in_expression(int next_free)override;
        std::unique_ptr<game_move> simplify(void)override;
        void accept(abstract_dispatcher& dispatcher)const override;
        uint priority(const options& opt)const override{return (opt.enabled_noop_after_modifier() or opt.enabled_noop_before_modifier()) ? 2 : 4;};
        std::string to_rbg(const options& opt, uint)const override;
        std::string to_rbg(const options&)const override;
        std::unique_ptr<game_move> flatten(void)override;
        straightness_result compute_k_straightness(StraightnessType st)const override;
        bool check_if_redundant(std::set<token>& assignments_so_far, bool&)const override;
        const token& get_left_side(void)const;
        const arithmetic_expression* get_right_side(void)const;
        bool is_lazy(void)const;
        std::unique_ptr<game_move> copy(void)const override;
        bool has_piece_as_variable(const std::set<token>& pieces)const override;
        bool is_modifier(void)const override;
};

std::unique_ptr<game_move> make_assignments_concatenation(
    std::vector<std::pair<token, std::unique_ptr<arithmetic_expression>>>&& legal_sides);

}

#endif
