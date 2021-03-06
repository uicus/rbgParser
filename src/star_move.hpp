#ifndef STAR_MOVE
#define STAR_MOVE

#include<memory>

#include"game_move.hpp"
#include"abstract_dispatcher.hpp"
#include"options.hpp"

namespace rbg_parser{

class star_move : public game_move{
        std::unique_ptr<game_move> contained_move;
        star_move(void)=default;
    public:
        star_move(std::unique_ptr<game_move>&& contained_move);
        ~star_move(void)override=default;
        star_move(star_move&&)=default;
        star_move(const star_move&)=delete;
        star_move& operator=(const star_move&)=delete;
        star_move& operator=(star_move&&)=default;
        int give_indices_in_expression(int next_free)override;
        bool modifies(void)const override;
        std::unique_ptr<game_move> simplify(void)override;
        void accept(abstract_dispatcher& dispatcher)const override;
        uint priority(const options& opt)const override{return opt.enabled_noop_before_star() ? 2 : 3;};
        std::string to_rbg(const options& opt, uint indent)const override;
        std::string to_rbg(const options& opt)const override;
        std::unique_ptr<game_move> flatten(void)override;
        straightness_result compute_k_straightness(StraightnessType st)const override;
        const game_move* get_content(void)const;
        std::unique_ptr<game_move> copy(void)const override;
        bool has_piece_as_variable(const std::set<token>& pieces)const override;
};

}

#endif
